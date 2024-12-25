use std::ops::Range;
use vulkano::descriptor_set::allocator::StandardDescriptorSetAllocator;
use vulkano::descriptor_set::{PersistentDescriptorSet, WriteDescriptorSet};
use vulkano::pipeline::{ComputePipeline, Pipeline, PipelineLayout, PipelineShaderStageCreateInfo};
use vulkano::pipeline::compute::ComputePipelineCreateInfo;
use vulkano::pipeline::layout::PipelineDescriptorSetLayoutCreateInfo;
use vulkano::sync::{self, GpuFuture};
use vulkano::command_buffer::allocator::{StandardCommandBufferAllocator, StandardCommandBufferAllocatorCreateInfo};
use vulkano::command_buffer::{AutoCommandBufferBuilder, CommandBufferUsage};
use vulkano::pipeline::PipelineBindPoint;



use crate::e_computing::EngineComputing;
mod e_computing;
mod cs;



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
    
}
