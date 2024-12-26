use pub_fields::pub_fields;
use crate::init_devices;
use vulkano::image::ImageUsage;
use vulkano::swapchain::{Swapchain, SwapchainCreateInfo};

#[pub_fields]
struct VkInitSwapchain {}


impl VkInitSwapchain {
    pub fn new(devices: init_devices::VkInitDevices ) -> VkInitSwapchain {
        
        let caps = devices.physical_device.surface_capabilities(&devices.surface, Default::default()).expect("Failed to get surface capabilities");
        
        let dimensions = devices.window.inner_size();
        
        let composite_alpha = caps.supported_composite_alpha.into_iter().next().unwrap();
        
        let image_format = devices.physical_device.surface_formats(&devices.surface, Default::default()).unwrap()[0].0;
        
        let (mut swapchain, images) = Swapchain::new(devices.logical_device.clone(), devices.surface.clone(), SwapchainCreateInfo {
            min_image_count: caps.min_image_count + 1,
            image_format,
            image_extent: dimensions.into(),
            image_usage: ImageUsage::COLOR_ATTACHMENT,
            composite_alpha,
            ..Default::default()
        }).unwrap();
        
        VkInitSwapchain {}
    }
}