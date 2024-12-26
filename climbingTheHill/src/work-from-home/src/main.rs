use climbing_the_hill::e_computing::EngineComputing;
use vulkano::buffer::BufferContents;
use vulkano::pipeline::graphics::vertex_input::Vertex;

#[derive(BufferContents, Vertex)]
#[repr(C)]
struct PVertex {
    #[format(R32G32_SFLOAT)]
    position: [f32; 2],
}

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

   
}
