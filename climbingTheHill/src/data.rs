use std::sync::Arc;
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::VulkanLibrary;
use pub_fields::pub_fields;
use vulkano::device::{DeviceExtensions, QueueFlags};
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::swapchain::Surface;
use winit::window::Window;
use winit::event_loop::EventLoop;

#[pub_fields]
pub struct EngineData{
    vk_library: Arc<VulkanLibrary>,
    vk_instance: Arc<Instance>,
    event_loop: Arc<EventLoop<()>>,
    window: Arc<Window>,
    surface: Arc<Surface>,
    physical_device: Arc<PhysicalDevice>,
}
impl EngineData {
    pub fn new() -> Self {
        let vk_library = Self::get_vk_library();
        let vk_instance = Self::get_vk_instance(vk_library.clone());
        let devices = Self::get_vk_physical_devices(vk_instance.clone());
        let event_loop = Arc::new(EventLoop::new().unwrap());
        #[allow(deprecated)]
        let window =  Arc::new(event_loop.create_window(Window::default_attributes()).unwrap());
        let surface = Surface::from_window(vk_instance.clone(), window.clone()).unwrap();
        
        let physical_device = Self::pick_physical_device(&devices,  surface.clone());
        Self {vk_library, vk_instance, event_loop, window, surface, physical_device}
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
}