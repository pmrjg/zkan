use std::sync::Arc;
use vulkano::instance::{Instance, InstanceCreateFlags, InstanceCreateInfo};
use vulkano::VulkanLibrary;
use pub_fields::pub_fields;
use vulkano::device::DeviceExtensions;
use vulkano::device::physical::{PhysicalDevice, PhysicalDeviceType};
use vulkano::swapchain::Surface;

#[pub_fields]
pub struct EngineData{
    vk_library: Arc<VulkanLibrary>,
    vk_instance: Arc<Instance>,
}
impl EngineData {
    pub fn new() -> Self {
        let vulkan_library = Self::get_vk_library();
        let vk_instance = Self::get_vk_instance(vulkan_library.clone());

        Self {vk_library, vk_instance}
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

    fn get_vk_physical_devices(instance: Arc<Instance>) -> Arc<impl ExactSizeIterator<Item = Arc<PhysicalDevice>>>{
        instance.enumerate_physical_devices().expect("failed to enumerate physical devices")
    }

    fn pick_physical_device(devices: &mut dyn ExactSizeIterator<Item = Arc<PhysicalDevice>>, surface: Arc<Surface>) -> Arc<PhysicalDevice> {
        let device_extensions = DeviceExtensions {
            khr_swapchain: true,
            ..DeviceExtensions::empty()
        };
        devices.filter(|device| device.supported_extensions().contains(&device_extensions))
            .filter_map(|device| {
                device.queue_family_properties()
                    .iter()
                    .enumerate()
                    .position(|i, d| {
                    d.queue_flags.graphics && device.surface_support(u32::from(i), &surface).unwrap_or(false)
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
        }).expect("No suitable physical device found").0
    }
}