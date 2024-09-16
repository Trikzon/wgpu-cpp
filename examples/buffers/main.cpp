#include <chrono>
#include <iostream>
#include <thread>

#include <wgpu.hpp>

int main()
{
    // Initalize WebGPU.
    const auto instance = wgpu::create_instance({});
    const auto adapter = instance.create_adapter({}).value();
    const auto device = adapter.create_device({}).value();

    // Create data on the CPU.
    std::vector<uint8_t> data(16);
    for (auto i = 0; i < 16; ++i)
    {
        data[i] = i;
    }
    std::cout << "CPU -> GPU: [";
    for (int i = 0; i < 16; ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << static_cast<int>(data[i]);
    }
    std::cout << "]" << std::endl;

    // Create a buffer to store the data.
    const auto buffer = device.create_buffer(
    {
        .label = "Data Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::CopySrc,
        .size = 16 * sizeof(uint8_t),
        .mapped_at_creation = false,
    });

    // Upload the data to the GPU.
    const auto queue = device.get_queue();
    queue.write_buffer(buffer, 0, data);

    // Copy the data from the GPU buffer to another buffer.
    const auto output_buffer = device.create_buffer(
    {
        .label = "Output Buffer",
        .usage = wgpu::BufferUsageFlags::CopyDst | wgpu::BufferUsageFlags::MapRead,
        .size = 16 * sizeof(uint8_t),
        .mapped_at_creation = false,
    });

    const auto command_encoder = device.create_command_encoder({.label = "Command Encoder"});
    command_encoder.copy_buffer_to_buffer(buffer, 0, output_buffer, 0, 16 * sizeof(uint8_t));

    const auto command_buffer = command_encoder.finish({.label = "Command Buffer"});
    queue.submit({command_buffer});

    struct Context
    {
        bool mapped = false;
        wgpu::Buffer buffer;
    };
    Context context{.buffer = output_buffer};

    const auto callback = output_buffer.map_async(wgpu::MapModeFlags::Read, 0, 16 * sizeof(uint8_t),
        [&context](const wgpu::BufferMapAsyncStatus status)
        {
            if (status != wgpu::BufferMapAsyncStatus::Success)
            {
                std::cerr << "Failed to map buffer." << std::endl;
                return;
            }

            const auto *data = context.buffer.get_const_mapped_range<uint8_t>(0, 16);

            std::cout << "GPU -> CPU: [";
            for (int i = 0; i < 16; ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << static_cast<int>(data[i]);
            }
            std::cout << "]" << std::endl;

            context.buffer.unmap();
            context.mapped = true;
        });

    while (!context.mapped)
    {
        device.tick();
    }

    return 0;
}
