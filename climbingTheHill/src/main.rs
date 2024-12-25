use crate::e_computing::EngineComputing;
mod e_computing;

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
        dst
    } = proc;
    
    let future = sync::now(logical_device.clone()).then_execute(queue.clone(), command_buffer).unwrap().then_signal_fence_and_flush().unwrap();
    
    future.wait(None).unwrap();
    let src_content = src.read().unwrap();
    let destination_content = dst.read().unwrap();
    
    assert_eq!(&*src_content, &*destination_content);
    
    println!("Everything succeeded!");
    
    
}
