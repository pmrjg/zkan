use climbing_the_hill::e_computing::EngineComputing;
use vulkano::buffer::{Buffer, BufferContents, BufferCreateInfo, BufferUsage};
use vulkano::format::Format;
use vulkano::image::view::ImageView;
use vulkano::memory::allocator::{AllocationCreateInfo, MemoryTypeFilter};
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

    let v1 = PVertex{ position: [-0.5, -0.5] };
    let v2 = PVertex{ position: [ 0.0, 0.5] };
    let v3 = PVertex{ position: [ 0.5, -0.25] };
    
    let vertex_buffer = Buffer::from_iter(
      memory_allocator.clone(),
      BufferCreateInfo {
          usage: BufferUsage::VERTEX_BUFFER,
          ..Default::default()
      },
      AllocationCreateInfo {
          memory_type_filter: MemoryTypeFilter::PREFER_DEVICE | MemoryTypeFilter::HOST_SEQUENTIAL_WRITE,
          ..Default::default()
      },
      vec![v1, v2, v3]
    ).unwrap();
    
    // Vertex Shader
    
    
    // Render pass
    let render_pass = vulkano::single_pass_renderpass!(
    logical_device.clone(),
    attachments: {
        color: {
            format: Format::R8G8B8A8_UNORM,
            samples: 1,
            load_op: Clear,
            store_op: Store,
        },
    },
    pass: {
        color: [color],
        depth_stencil: {},
    },
)
        .unwrap();

}
