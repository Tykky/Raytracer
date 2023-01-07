#ifndef RAYTRACER_GRAPHICS_H
#define RAYTRACER_GRAPHICS_H

namespace RT
{
	struct Texture
	{
		void* resource;
	};

	struct gfxDevice
	{
		struct VirtualDevice
		{
			virtual ~virtualDevice() = default;
			virtual Texture createTexture() = 0;
		};

		template<typename T>
		struct RealDevice : public VirtualDevice
		{
			RealDevice(T devie) :
				m_device(device) {}
		private:
			T m_device;
		};
	};
}

#endif // RAYTRACER_GRAPHICS_H
