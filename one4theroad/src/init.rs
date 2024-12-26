use std::sync::Arc;
use pub_fields::pub_fields;
use vulkano::device::{Device, DeviceExtensions, QueueFlags};
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::swapchain::{Surface, SurfaceInfo};
use vulkano::VulkanLibrary;
use winit::event_loop::EventLoop;
use winit::window::{Window, WindowAttributes};

#[pub_fields]
struct VkInit {
    library: Arc<VulkanLibrary>,
    instance: Arc<Instance>,
    physical_device: Arc<PhysicalDevice>,
    window: Arc<Window>,
    surface: Arc<Surface>,
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

        let window = Arc::new(event_loop.create_window(wa).unwrap());

        let surface = Surface::from_window(instance.clone(), window.clone()).unwrap();

        let device_extensions = DeviceExtensions {
            khr_swapchain: true,
            ..DeviceExtensions::empty()
        };

        let physical_device = Self::pick_physical_device(instance.clone(), &device_extensions, surface.clone());

        Self {library: l, instance, physical_device, window, surface}
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
}