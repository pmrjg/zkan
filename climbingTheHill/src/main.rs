use std::ops::Range;
use vulkano::buffer::{Buffer, BufferCreateInfo, BufferUsage};
use vulkano::descriptor_set::allocator::StandardDescriptorSetAllocator;
use vulkano::descriptor_set::{PersistentDescriptorSet, WriteDescriptorSet};
use vulkano::pipeline::{ComputePipeline, Pipeline, PipelineLayout, PipelineShaderStageCreateInfo};
use vulkano::pipeline::compute::ComputePipelineCreateInfo;
use vulkano::pipeline::layout::PipelineDescriptorSetLayoutCreateInfo;
use vulkano::sync::{self, GpuFuture};
use vulkano::command_buffer::allocator::{StandardCommandBufferAllocator, StandardCommandBufferAllocatorCreateInfo};
use vulkano::command_buffer::{AutoCommandBufferBuilder, CommandBufferUsage};
use vulkano::pipeline::PipelineBindPoint;
use vulkano::image::{Image, ImageCreateInfo, ImageType, ImageUsage};
use vulkano::format::Format;
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter};
use vulkano::command_buffer::ClearColorImageInfo;
use vulkano::format::ClearColorValue;
use vulkano::command_buffer::CopyImageToBufferInfo;
use image::{ImageBuffer, Rgba};
use vulkano::image::view::ImageView;

use crate::e_computing::EngineComputing;
pub mod e_computing;
mod cs;
mod shader_fractal;

fn main() {
    let proc = EngineComputing::new();

    let EngineComputing
    {
        vk_library,
        vk_instance,
        event_loop,
        window,
        surface,
        physical_device,
        queue_family_index,
        logical_device,
        queue,
        memory_allocator,
        command_buffer_allocator,
        command_buffer,
        src,
        dst,
        data_buffer, 
        db
    } = proc;
    
    let future = sync::now(logical_device.clone()).then_execute(queue.clone(), command_buffer).unwrap().then_signal_fence_and_flush().unwrap();
    
    future.wait(None).unwrap();
    let src_content = src.read().unwrap();
    let destination_content = dst.read().unwrap();
    
    assert_eq!(&*src_content, &*destination_content);
    
    println!("Everything succeeded!");
    
    let shader = cs::cs::load(logical_device.clone()).expect("failed to create shader module");
    let ep = shader.entry_point("main").unwrap();
    let stage = PipelineShaderStageCreateInfo::new(ep);
    
    let layout = PipelineLayout::new(logical_device.clone(), PipelineDescriptorSetLayoutCreateInfo::from_stages([&stage]).into_pipeline_layout_create_info(logical_device.clone()).unwrap()).unwrap();
    
    let compute_pipeline = ComputePipeline::new(logical_device.clone(), None, ComputePipelineCreateInfo::stage_layout(stage, layout)).expect("failed to create compute pipeline");
    
    let descriptor_set_allocator = StandardDescriptorSetAllocator::new(logical_device.clone(), Default::default());
    let pipeline_layout = compute_pipeline.layout();
    let descriptor_set_layouts = pipeline_layout.set_layouts();
    
    let descriptor_set_layout_index = 0;
    let descriptor_set_layout = descriptor_set_layouts.get(descriptor_set_layout_index).unwrap();
    let descriptor_set = PersistentDescriptorSet::new(&descriptor_set_allocator, descriptor_set_layout.clone(), [WriteDescriptorSet::buffer(0, db.clone())], []).unwrap();
    
    let command_buffer_allocator = StandardCommandBufferAllocator::new(logical_device.clone(), StandardCommandBufferAllocatorCreateInfo::default());
    
    let mut command_buffer_builder = AutoCommandBufferBuilder::primary(&command_buffer_allocator, queue.queue_family_index(), CommandBufferUsage::OneTimeSubmit).unwrap();
    
    let work_group_counts =  [1024, 1, 1];
    
    command_buffer_builder.bind_pipeline_compute(compute_pipeline.clone()).unwrap()
        .bind_descriptor_sets(PipelineBindPoint::Compute, compute_pipeline.layout().clone(), descriptor_set_layout_index as u32, descriptor_set)
        .unwrap()
        .dispatch(work_group_counts)
        .unwrap();
    
    let cb = command_buffer_builder.build().unwrap();
    
    let future = sync::now(logical_device.clone()).then_execute(queue.clone(), cb).unwrap().then_signal_fence_and_flush().unwrap();
    
    future.wait(None).unwrap();
    
    let content = data_buffer.read().unwrap();
    
    let values: Range<u32> = 0..65536u32;
    for (n, val) in content.iter().enumerate() {
        assert_eq!(*val, n as u32 * 12);
    }
    
    println!("Computing 65536 values on GPU succeeded");

    let image = Image::new(
        memory_allocator.clone(),
        ImageCreateInfo {
            image_type: ImageType::Dim2d,
            format: Format::R8G8B8A8_UNORM,
            extent: [1024, 1024, 1],
            usage: ImageUsage::TRANSFER_DST | ImageUsage::TRANSFER_SRC,
            ..Default::default()
        },
        AllocationCreateInfo {
            memory_type_filter: MemoryTypeFilter::PREFER_DEVICE,
            ..Default::default()
        },
    )
        .unwrap();

    let mut builder = AutoCommandBufferBuilder::primary(
        &command_buffer_allocator,
        queue.queue_family_index(),
        CommandBufferUsage::OneTimeSubmit,
    )
        .unwrap();

    let buf = Buffer::from_iter(
        memory_allocator.clone(),
        BufferCreateInfo {
            usage: BufferUsage::TRANSFER_DST,
            ..Default::default()
        },
        AllocationCreateInfo {
            memory_type_filter: MemoryTypeFilter::PREFER_HOST
                | MemoryTypeFilter::HOST_RANDOM_ACCESS,
            ..Default::default()
        },
        (0..1024 * 1024 * 4).map(|_| 0u8),
    )
        .expect("failed to create buffer");

    builder
        .clear_color_image(ClearColorImageInfo {
            clear_value: ClearColorValue::Float([0.0, 0.0, 1.0, 1.0]),
            ..ClearColorImageInfo::image(image.clone())
        })
        .unwrap().copy_image_to_buffer(CopyImageToBufferInfo::image_buffer(image.clone(), buf.clone())).unwrap();;

    let command_buffer = builder.build().unwrap();
    
    let future = sync::now(logical_device.clone()).then_execute(queue.clone(), command_buffer).unwrap().then_signal_fence_and_flush().unwrap();
    
    future.wait(None).unwrap();
    
    let buffer_content = buf.read().unwrap();
    let image = ImageBuffer::<Rgba<u8>, _>::from_raw(1024, 1024, &buffer_content[..]).unwrap();
    
    image.save("image.png").unwrap();
    println!("Printing image succeeded!");

    let shader = shader_fractal::fractal::load(logical_device.clone()).expect("failed to create shader module");
    let ep = shader.entry_point("main").unwrap();
    let stage = PipelineShaderStageCreateInfo::new(ep);

    let layout = PipelineLayout::new(logical_device.clone(), PipelineDescriptorSetLayoutCreateInfo::from_stages([&stage]).into_pipeline_layout_create_info(logical_device.clone()).unwrap()).unwrap();

    let compute_pipeline = ComputePipeline::new(logical_device.clone(), None, ComputePipelineCreateInfo::stage_layout(stage, layout)).expect("failed to create compute pipeline");


    let image2 = Image::new(
        memory_allocator.clone(),
        ImageCreateInfo {
            image_type: ImageType::Dim2d,
            format: Format::R8G8B8A8_UNORM,
            extent: [1024, 1024, 1],
            usage: ImageUsage::STORAGE | ImageUsage::TRANSFER_SRC,
            ..Default::default()
        },
        AllocationCreateInfo {
            memory_type_filter: MemoryTypeFilter::PREFER_DEVICE,
            ..Default::default()
        },
    ).unwrap();
    
    let view = ImageView::new_default(image2.clone()).unwrap();
    let layout1 = compute_pipeline.layout().set_layouts().get(0).unwrap();
    let set = PersistentDescriptorSet::new(
        &descriptor_set_allocator,
        layout1.clone(),
        [WriteDescriptorSet::image_view(0, view.clone())], // 0 is the binding
        [],
    )
        .unwrap();

    let buf1 = Buffer::from_iter(
        memory_allocator.clone(),
        BufferCreateInfo {
            usage: BufferUsage::TRANSFER_DST,
            ..Default::default()
        },
        AllocationCreateInfo {
            memory_type_filter: MemoryTypeFilter::PREFER_HOST
                | MemoryTypeFilter::HOST_RANDOM_ACCESS,
            ..Default::default()
        },
        (0..1024 * 1024 * 4).map(|_| 0u8),
    ).expect("failed to create buffer");

    let mut builder1 = AutoCommandBufferBuilder::primary(
        &command_buffer_allocator,
        queue.queue_family_index(),
        CommandBufferUsage::OneTimeSubmit,
    )
        .unwrap();

    builder1
        .bind_pipeline_compute(compute_pipeline.clone())
        .unwrap()
        .bind_descriptor_sets(
            PipelineBindPoint::Compute,
            compute_pipeline.layout().clone(),
            0,
            set,
        )
        .unwrap()
        .dispatch([1024 / 8, 1024 / 8, 1])
        .unwrap()
        .copy_image_to_buffer(CopyImageToBufferInfo::image_buffer(
            image2.clone(),
            buf1.clone(),
        ))
        .unwrap();

    let command_buffer2 = builder1.build().unwrap();
    
    let future = sync::now(logical_device.clone()).then_execute(queue.clone(), command_buffer2).unwrap().then_signal_fence_and_flush().unwrap();
    
    future.wait(None).unwrap();
    
    let buffer_content = buf1.read().unwrap();
    let image = ImageBuffer::<Rgba<u8>,_>::from_raw(1024, 1024, &buffer_content[..]).unwrap();
    image.save("mandelbrot.png").unwrap();
    
    println!("Writing image succeeded!");
    
}
