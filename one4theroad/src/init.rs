use std::sync::Arc;
use pub_fields::pub_fields;
use vulkano::device::{Device, DeviceCreateInfo, DeviceExtensions, Queue, QueueCreateInfo, QueueFlags};
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::swapchain::{Surface};
use vulkano::VulkanLibrary;
use winit::event_loop::EventLoop;
use winit::window::{Window};

#[pub_fields]
struct VkInit {
    library: Arc<VulkanLibrary>,
    instance: Arc<Instance>,
    physical_device: Arc<PhysicalDevice>,
    window: Arc<Window>,
    surface: Arc<Surface>,
    event_loop: Arc<EventLoop<()>>,
    logical_device: Arc<Device>,
    queue_index: Arc<u32>,
    queue: Arc<Queue>,
}

impl VkInit {
    fn new() -> Self{
        let l = VulkanLibrary::new().expect("Failed to load vulkan library!");

        let event_loop = EventLoop::new().unwrap();
        let required_extensions = Surface::required_extensions(&event_loop);
        let instance = Instance::new(l.clone(), InstanceCreateInfo {
            flags: InstanceCreateFlags::ENUMERATE_PORTABILITY,
            enabled_extensions: required_extensions,
            ..Default::default()
        }).unwrap();

        let wa = Window::default_attributes();

        #[allow(deprecated)]
        let window = Arc::new(event_loop.create_window(wa).unwrap());

        let surface = Surface::from_window(instance.clone(), window.clone()).unwrap();

        let device_extensions = DeviceExtensions {
            khr_swapchain: true,
            ..DeviceExtensions::empty()
        };

        let physical_device = Self::pick_physical_device(instance.clone(), &device_extensions, surface.clone());
        
        let queue_index = Self::get_device_queue_index(&physical_device);
        let (logical_device, mut queues) = Self::logical_device_and_queues(physical_device.clone(), &queue_index);
        
        let queue = queues.next().expect("Failed to get queue");

        Self {library: l, instance, physical_device, window, surface, event_loop: Arc::new(event_loop), logical_device, queue_index: Arc::new(queue_index), queue}
    }

    fn pick_physical_device(instance: Arc<Instance>, device_extensions: &DeviceExtensions, surface: Arc<Surface>) -> Arc<PhysicalDevice>{
        let x = instance.enumerate_physical_devices().expect("failed to enumerate physical devices");
        let devices = Vec::from_iter(x);

        devices.iter().filter(|device| device.supported_extensions().contains(device_extensions))
            .filter_map(|device| {
                device.queue_family_properties()
                    .iter()
                    .enumerate()
                    .position(|(i, d)| {
                        d.queue_flags.contains(QueueFlags::COMPUTE | QueueFlags::GRAPHICS) && device.surface_support(i as u32, &surface).unwrap_or(false)
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

    fn get_device_queue_index(physical_device: &PhysicalDevice) -> u32 {
        physical_device.queue_family_properties()
            .iter().enumerate()
            .position(|(_qf_idx, qf_props)| {
                qf_props.queue_flags.contains(QueueFlags::GRAPHICS | QueueFlags::COMPUTE)
            })
            .expect("no such queue family") as u32
    }
    
    fn logical_device_and_queues(physical_device: Arc<PhysicalDevice>, index: &u32) -> (Arc<Device>, impl ExactSizeIterator<Item=Arc<Queue>> + Sized) {
        Device::new(
            physical_device, DeviceCreateInfo {
                queue_create_infos: vec![QueueCreateInfo{
                    queue_family_index: *index, ..Default::default()
                }],
                ..Default::default()
            }
        ).expect("failed to create device")
    }
}