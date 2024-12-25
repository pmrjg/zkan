use std::sync::Arc;
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::VulkanLibrary;
use pub_fields::pub_fields;
use vulkano::device::{Device, DeviceCreateInfo, DeviceExtensions, Queue, QueueCreateInfo, QueueFlags};
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::swapchain::Surface;
use winit::window::Window;
use winit::event_loop::EventLoop;

use vulkano::memory::allocator::{StandardMemoryAllocator, AllocationCreateInfo, MemoryTypeFilter};
use vulkano::buffer::{Buffer, BufferContents, BufferCreateInfo, BufferUsage, Subbuffer};

#[pub_fields]
pub struct EngineComputing{
    vk_library: Arc<VulkanLibrary>,
    vk_instance: Arc<Instance>,
    event_loop: Arc<EventLoop<()>>,
    window: Arc<Window>,
    surface: Arc<Surface>,
    physical_device: Arc<PhysicalDevice>,
    queue_family_index: Arc<u32>,
    logical_device: Arc<Device>,
    queue: Arc<Queue>,
    memory_allocator: Arc<StandardMemoryAllocator>,
}
impl EngineComputing {
    pub fn new() -> Self {
        let vk_library = Self::get_vk_library();
        let vk_instance = Self::get_vk_instance(vk_library.clone());
        let devices = Self::get_vk_physical_devices(vk_instance.clone());
        let event_loop = Arc::new(EventLoop::new().unwrap());
        #[allow(deprecated)]
        let window =  Arc::new(event_loop.create_window(Window::default_attributes()).unwrap());
        let surface = Surface::from_window(vk_instance.clone(), window.clone()).unwrap();

        let physical_device = Self::pick_physical_device(&devices,  surface.clone());

        let queue_family_index = Self::get_device_queue_index(&physical_device) as u32;

        let (logical_device, mut queues) = Self::create_logical_device(physical_device.clone(), &queue_family_index);

        
        let queue = queues.next().unwrap();
        
        let memory_allocator = Arc::new(StandardMemoryAllocator::new_default(logical_device.clone()));

        Self {vk_library, 
            vk_instance, 
            event_loop, window, 
            surface, 
            physical_device, 
            queue_family_index: Arc::new(queue_family_index),
            logical_device,
            queue,
            memory_allocator,
        }
    }

    fn get_vk_library() -> Arc<VulkanLibrary> {
        VulkanLibrary::new().expect("no local vulkan library/dll")
    }

    fn get_vk_instance(library: Arc<VulkanLibrary>) -> Arc<Instance> {
        Instance::new(library, InstanceCreateInfo {
            flags: InstanceCreateFlags::ENUMERATE_PORTABILITY,
            ..Default::default()
        }).expect("failed to create instance")
    }

    fn get_vk_physical_devices(instance: Arc<Instance>) -> Vec<Arc<PhysicalDevice>> {
        let x = instance.enumerate_physical_devices().expect("failed to enumerate physical devices");
        Vec::from_iter(x)
    }

    fn get_device_queue_index(physical_device: &PhysicalDevice) -> usize {
        physical_device.queue_family_properties()
            .iter().enumerate()
            .position(|(_qf_idx, qf_props)| {
                qf_props.queue_flags.contains(QueueFlags::GRAPHICS)
            })
            .expect("no such queue family")
    }

    fn pick_physical_device(devices: &Vec<Arc<PhysicalDevice>>, surface: Arc<Surface>) -> Arc<PhysicalDevice> {
        let device_extensions = DeviceExtensions {
            khr_swapchain: true,
            ..DeviceExtensions::empty()
        };
        devices.iter().filter(|device| device.supported_extensions().contains(&device_extensions))
            .filter_map(|device| {
                device.queue_family_properties()
                    .iter()
                    .enumerate()
                    .position(|(i, d)| {
                    d.queue_flags.contains(QueueFlags::GRAPHICS) && device.surface_support(i as u32, &surface).unwrap_or(false)
                }).map(|i| (device, i as u32))
            }).min_by_key(|(p, _)| {
            match p.properties().device_type {
                PhysicalDeviceType::DiscreteGpu => 0,
                PhysicalDeviceType::IntegratedGpu => 1,
                PhysicalDeviceType::VirtualGpu => 2,
                PhysicalDeviceType::Cpu => 3,
                PhysicalDeviceType::Other => 4,
                _ => 5
            }
        }).expect("No suitable physical device found").0.clone()
    }

    fn create_logical_device(physical_device: Arc<PhysicalDevice>, index: &u32) -> (Arc<Device>, impl ExactSizeIterator<Item=Arc<Queue>> + Sized) {
        let (device, queues) = Device::new(
            physical_device, DeviceCreateInfo {
                queue_create_infos: vec![QueueCreateInfo{
                    queue_family_index: *index, ..Default::default()
                }],
                ..Default::default()
            }
        ).expect("failed to create device");

        (device, queues)

    }
    
    pub fn create_buffer<T>(&self, data: T)-> Subbuffer<T> where T:BufferContents, T:Clone {
        Buffer::from_data(
            self.memory_allocator.clone(),
            BufferCreateInfo {
                usage: BufferUsage::UNIFORM_BUFFER,
                ..Default::default()
            },
            AllocationCreateInfo {
                memory_type_filter: MemoryTypeFilter::PREFER_DEVICE | MemoryTypeFilter::HOST_SEQUENTIAL_WRITE, ..Default::default()
            },
            data
        ).unwrap()
    }
}