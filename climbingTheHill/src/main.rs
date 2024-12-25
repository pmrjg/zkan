use std::ops::Deref;
use vulkano::buffer::Buffer;
use vulkano::descriptor_set::allocator::StandardDescriptorSetAllocator;
use vulkano::descriptor_set::{PersistentDescriptorSet, WriteDescriptorSet};
use vulkano::pipeline::{ComputePipeline, Pipeline, PipelineLayout, PipelineShaderStageCreateInfo};
use vulkano::pipeline::compute::ComputePipelineCreateInfo;
use vulkano::pipeline::layout::PipelineDescriptorSetLayoutCreateInfo;
use crate::e_computing::EngineComputing;
mod e_computing;
mod cs;

use vulkano::sync::{self, GpuFuture};

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
    
}
