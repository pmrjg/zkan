
pub mod fractal {
    use vulkano_shaders::shader;

    shader!{
        ty: "compute",
        path: "src/shaders/fractal.glsl",
    }
}