use std::sync::Arc;
use pub_fields::pub_fields;
use vulkano::format::Format;
use crate::init_devices;
use vulkano::image::{Image, ImageUsage};
use vulkano::swapchain::{CompositeAlpha, SurfaceCapabilities, Swapchain, SwapchainCreateInfo};
use winit::dpi::PhysicalSize;

#[pub_fields]
pub struct VkInitSwapchain {
    caps: Arc<SurfaceCapabilities>,
    dimensions: Arc<PhysicalSize<u32>>,
    composite_alpha: CompositeAlpha,
    image_format: Format,
    swapchain: Arc<Swapchain>,
    images: Vec<Arc<Image>>
}

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
        
        VkInitSwapchain {caps: Arc::new(caps), dimensions: Arc::new(dimensions), composite_alpha, image_format, swapchain, images}
    }
}