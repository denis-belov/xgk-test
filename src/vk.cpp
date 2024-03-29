#define DEBUG



#include <iostream>
#include <cstring>
#include <vector>

#if defined(__linux__)
	#define GLFW_EXPOSE_NATIVE_X11
	#define VK_USE_PLATFORM_XLIB_KHR
	#include <X11/Xlib.h>
	#include <dlfcn.h>
#elif defined(_WIN64)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#include "xgk-aux/src/api/vulkan.h"

// #if defined(__GNUC__)
// 	#include "vertex_shader_code_vulkan.h"
// 	#include "fragment_shader_code_vulkan.h"
// #else
// 	#include "vertex_shader_code_vulkan.h"
// 	#include "fragment_shader_code_vulkan.h"
// #endif



using std::cout;
using std::endl;



const uint32_t vertex_shader_code_vulkan []
{
	#include "vertex_shader_code_vulkan.h"
};
const uint32_t fragment_shader_code_vulkan []
{
	#include "fragment_shader_code_vulkan.h"
};



extern const float vertices [];
extern const uint32_t vertices_size;
extern void (* loop_function) (void);
extern void (* destroy_api_function) (void);
extern GLFWwindow* window;

struct Orbit;

extern Orbit orbit;

void idle_function (void);
void glfw_key_callback (GLFWwindow*, int, int, int, int);



using namespace XGK::VULKAN;

Instance vk_inst;
VkSurfaceKHR vk_surf { VK_NULL_HANDLE };
Device vk_dev;
VkQueue vk_graphics_queue { VK_NULL_HANDLE };
VkQueue vk_present_queue { VK_NULL_HANDLE };

uint64_t vk_swapchain_image_count {};

void* vk_uniform_buffer_mem_addr {};
std::vector<VkFence> vk_submission_completed_fences;
VkSwapchainKHR vk_swapchain { VK_NULL_HANDLE };
VkRenderPass vk_render_pass { VK_NULL_HANDLE };
std::vector<VkFramebuffer> vk_framebuffers;
std::vector<VkSemaphore> vk_image_available_semaphores;
std::vector<VkSemaphore> vk_submission_completed_semaphores;
std::vector<uint32_t> vk_image_indices;
std::vector<VkSubmitInfo> vk_submit_i;
std::vector<VkPresentInfoKHR> vk_present_i;
std::vector<VkRenderPassBeginInfo> vk_render_pass_bi;
std::vector<VkCommandBuffer> vk_cmd_buffers;
VkClearValue clear_value [] { {}, {} };
std::vector<VkDescriptorSet> vk_descr_set;
VkPipelineLayout vk_ppl_layout { VK_NULL_HANDLE };
VkPipeline vk_ppl { VK_NULL_HANDLE };
// VkPipelineLayout vk_ppl_layout2 { VK_NULL_HANDLE };
VkPipeline vk_ppl2 { VK_NULL_HANDLE };
VkBuffer vk_vertex_buffer { VK_NULL_HANDLE };
uint32_t curr_image {};



void loop_function_VK (void)
{
	memcpy(vk_uniform_buffer_mem_addr + 64, ((void*) &orbit) + 64, 64);

	// causes validation error without crashing, has to be global static
	// static uint32_t curr_image {};

	vkAcquireNextImageKHR(vk_dev.handle, vk_swapchain, 0xFFFFFFFF, vk_image_available_semaphores[curr_image], VK_NULL_HANDLE, &vk_image_indices[curr_image]);

	// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	static const VkCommandBufferBeginInfo vk_command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT) };

	static const VkDeviceSize vk_vertex_buffer_offset {};

	vkWaitForFences(vk_dev.handle, 1, &vk_submission_completed_fences[curr_image], VK_TRUE, 0xFFFFFFFF);

	vkBeginCommandBuffer(vk_cmd_buffers[curr_image], &vk_command_buffer_bi);



	vkCmdBindDescriptorSets(vk_cmd_buffers[curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, vk_ppl_layout, 0, 1, &vk_descr_set[0], 0, nullptr);

	vkCmdBindVertexBuffers(vk_cmd_buffers[curr_image], 0, 1, &vk_vertex_buffer, &vk_vertex_buffer_offset);
	vkCmdBeginRenderPass(vk_cmd_buffers[curr_image], &vk_render_pass_bi[curr_image], VK_SUBPASS_CONTENTS_INLINE);
	// vkCmdBindDescriptorSets(vk_cmd_buffers[curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, vk_ppl_layout, 0, 1, &vk_descr_set[0], 0, nullptr);
	vkCmdBindPipeline(vk_cmd_buffers[curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, vk_ppl);
	vkCmdDraw(vk_cmd_buffers[curr_image], vertices_size / 12, 1, 0, 0);

	vkCmdBindPipeline(vk_cmd_buffers[curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, vk_ppl2);
	vkCmdDraw(vk_cmd_buffers[curr_image], 1, 1, 0, 0);



	vkCmdEndRenderPass(vk_cmd_buffers[curr_image]);
	vkEndCommandBuffer(vk_cmd_buffers[curr_image]);

	vkResetFences(vk_dev.handle, 1, &vk_submission_completed_fences[curr_image]);

	vkQueueSubmit(vk_graphics_queue, 1, &vk_submit_i[curr_image], vk_submission_completed_fences[curr_image]);

	vk_present_i[curr_image].pImageIndices = &vk_image_indices[curr_image];

	vkQueuePresentKHR(vk_present_queue, &vk_present_i[curr_image]);

	const static uint64_t max_swapchain_image_index { vk_swapchain_image_count - 1 };

	if (++curr_image > max_swapchain_image_index)
	{
		curr_image = 0;
	}
};

void destroyVK (void)
{
	loop_function = idle_function;

	vkDeviceWaitIdle(vk_dev.handle);

	vk_dev.destroy();

	vk_inst.destroy();

	glfwDestroyWindow(window);

	glfwTerminate();
};

void initVK (void)
{
	if (destroy_api_function != destroyVK)
	{
		// destroy current context

		destroy_api_function();



		// glfw

		glfwInit();

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(800, 600, "", nullptr, nullptr);

		glfwSetKeyCallback(window, glfw_key_callback);
		glfwMakeContextCurrent(window);



		// base vulkan

		curr_image = 0;

		const char* vk_inst_exts []
		#ifdef DEBUG
			#if defined(__linux__)
				{ "VK_KHR_surface", "VK_KHR_xlib_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
			#elif defined(_WIN64)
				{ "VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
			#endif
		#else
			#if defined(__linux__)
				{ "VK_KHR_surface", "VK_KHR_xlib_surface" };
			#elif defined(_WIN64)
				{ "VK_KHR_surface", "VK_KHR_win32_surface" };
			#endif
		#endif

		VkApplicationInfo app_i { AppI() };

		#ifdef DEBUG
			const char* vk_inst_layers [] { "VK_LAYER_KHRONOS_validation" };

			vk_inst.create(&app_i, 1, vk_inst_layers, 3, vk_inst_exts);
		#else
			vk_inst.create(&app_i, 0, nullptr, 2, vk_inst_exts);
		#endif

		// cout << vk_inst.physical_devices << endl;

		VkPhysicalDevice vk_physical_device { vk_inst.physical_devices[0] };
		// VkPhysicalDeviceProperties pProperties = {};
		// vkGetPhysicalDeviceProperties(vk_physical_device, &pProperties);
		// cout << vk_inst.physical_device_count << endl;
		// cout << pProperties.apiVersion << endl;
		// cout << pProperties.driverVersion << endl;
		// cout << pProperties.vendorID << endl;
		// cout << pProperties.deviceID << endl;
		// cout << pProperties.deviceType << endl;
		// cout

		vk_surf =
		#if defined(__linux__)
			vk_inst.SurfaceKHR(glfwGetX11Display(), glfwGetX11Window(window));
		#elif defined(_WIN64)
			vk_inst.SurfaceKHR(GetModuleHandle(nullptr), glfwGetWin32Window(window));
		#endif

		const char* vk_dev_exts [] { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		vk_dev.getProps(vk_physical_device, vk_surf);

		static const float queue_priorities { 1.0f };

		std::vector<VkDeviceQueueCreateInfo> queue_ci { DevQueueCI(vk_dev.graphics_queue_family_index, 1, &queue_priorities) };

		if (vk_dev.graphics_queue_family_index != vk_dev.present_queue_family_index)
		{
			queue_ci.push_back(DevQueueCI(vk_dev.present_queue_family_index, 1, &queue_priorities));
		}

		// cout << "G " << vk_dev.graphics_queue_family_index << endl;
		// cout << "P " <<  vk_dev.present_queue_family_index << endl;

		vk_dev.create(vk_physical_device, vk_dev.graphics_queue_family_index != vk_dev.present_queue_family_index ? 2 : 1, queue_ci.data(), 0, nullptr, 1, vk_dev_exts);

		vk_graphics_queue = vk_dev.Queue(vk_dev.graphics_queue_family_index, 0);
		vk_present_queue = vk_dev.Queue(vk_dev.present_queue_family_index, 0);



		// render pass

		// objects accessed by render pass
		VkAttachmentDescription vk_render_pass_attach []
		{
			// color
			{
				0,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_SAMPLE_COUNT_1_BIT,
				// VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			},


			// depth
			{
				0,
				VK_FORMAT_D32_SFLOAT,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			},

			// // color_resolve
			// {
			//   0,
			//   VK_FORMAT_B8G8R8A8_UNORM,
			//   VK_SAMPLE_COUNT_1_BIT,
			//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE,
			//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
			//   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			// },
		};

		// object accessed by subpass
		VkAttachmentReference color_attach_ref { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		VkAttachmentReference depth_attach_ref { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
		// VkAttachmentReference color_attach_resolve_ref = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkSubpassDescription subpass_desc
		{
			0, VK_PIPELINE_BIND_POINT_GRAPHICS,
			0, nullptr,
			// 1, &color_attach_ref, &color_attach_resolve_ref, &depth_attach_ref
			1, &color_attach_ref, nullptr, &depth_attach_ref
		};

		VkSubpassDependency subpass_dep
		{
			VK_SUBPASS_EXTERNAL, 0,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		};

		vk_render_pass =
			vk_dev.RenderPass
			(
				// 3, vk_render_pass_attach,
				2, vk_render_pass_attach,
				1, &subpass_desc,
				1, &subpass_dep
			);



		//

		const uint32_t qfi [] { vk_dev.graphics_queue_family_index, vk_dev.present_queue_family_index };

		vk_swapchain = vk_dev.SwapchainKHR
		(
			vk_surf,
			8,
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
			800, 600,
			1,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			// VK_SHARING_MODE_EXCLUSIVE,
			// 0, nullptr,
			VK_SHARING_MODE_CONCURRENT,
			2, qfi,
			VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR,
			// VK_PRESENT_MODE_FIFO_KHR,
			// VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			VK_TRUE
		);

		std::vector<VkImage> vk_swapchain_images { vk_dev.getSwapchainImages(vk_swapchain) };

		vk_swapchain_image_count = vk_swapchain_images.size();

		std::vector<VkImageView> vk_swapchain_image_views(vk_swapchain_image_count);
		std::vector<VkImage> vk_render_images(vk_swapchain_image_count);
		VkMemoryRequirements vk_render_image_mem_reqs {};
		uint64_t vk_render_image_dev_local_mem_index {};
		std::vector<VkDeviceMemory> vk_render_image_mems(vk_swapchain_image_count);
		std::vector<VkImageView> vk_render_image_views(vk_swapchain_image_count);
		vk_framebuffers.resize(vk_swapchain_image_count);
		vk_submission_completed_fences.resize(vk_swapchain_image_count);
		vk_submission_completed_semaphores.resize(vk_swapchain_image_count);
		vk_image_available_semaphores.resize(vk_swapchain_image_count);

		std::vector<VkImage> vk_depth_images(vk_swapchain_image_count);
		std::vector<VkImageView> vk_depth_image_views(vk_swapchain_image_count);
		VkMemoryRequirements vk_depth_image_mem_reqs {};
		uint64_t vk_depth_image_dev_local_mem_index {};
		std::vector<VkDeviceMemory> vk_depth_image_mems(vk_swapchain_image_count);

		for (uint64_t i = 0; i < vk_swapchain_image_count; ++i)
		{
			vk_swapchain_image_views[i] = vk_dev.ImageView
			(
				vk_swapchain_images[i],
				VK_IMAGE_VIEW_TYPE_2D,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_IMAGE_ASPECT_COLOR_BIT,
				0, 1,
				0, 1
			);

			vk_render_images[i] = vk_dev.Image
			(
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_B8G8R8A8_UNORM,
				800, 600, 1,
				1,
				1,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0, nullptr,
				VK_IMAGE_LAYOUT_UNDEFINED
			);

			if (!i)
			{
				vk_render_image_mem_reqs = vk_dev.MemReqs(vk_render_images[i]);

				vk_render_image_dev_local_mem_index = vk_dev.getMemTypeIndex(&vk_render_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			}

			vk_render_image_mems[i] = vk_dev.Mem(vk_render_image_mem_reqs.size, vk_render_image_dev_local_mem_index);

			vk_dev.bindMem(vk_render_images[i], vk_render_image_mems[i]);

			vk_render_image_views[i] = vk_dev.ImageView
			(
				vk_render_images[i],
				VK_IMAGE_VIEW_TYPE_2D,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_IMAGE_ASPECT_COLOR_BIT,
				0, 1,
				0, 1
			);



			vk_depth_images[i] = vk_dev.Image
			(
				VK_IMAGE_TYPE_2D,
				VK_FORMAT_D32_SFLOAT,
				800, 600, 1,
				1,
				1,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0, nullptr,
				VK_IMAGE_LAYOUT_UNDEFINED
			);

			if (!i)
			{
				vk_depth_image_mem_reqs = vk_dev.MemReqs(vk_depth_images[i]);

				vk_depth_image_dev_local_mem_index = vk_dev.getMemTypeIndex(&vk_depth_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			}

			vk_depth_image_mems[i] = vk_dev.Mem(vk_depth_image_mem_reqs.size, vk_depth_image_dev_local_mem_index);

			vk_dev.bindMem(vk_depth_images[i], vk_depth_image_mems[i]);

			vk_depth_image_views[i] = vk_dev.ImageView
			(
				vk_depth_images[i],
				VK_IMAGE_VIEW_TYPE_2D,
				VK_FORMAT_D32_SFLOAT,
				VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_IMAGE_ASPECT_DEPTH_BIT,
				0, 1,
				0, 1
			);



			// VkImageView vk_framebuffer_attach[] = { vk_render_image_views[i], vk_depth_image_views[i], vk_swapchain_image_views[i] };
			// VkImageView vk_framebuffer_attach[] = { vk_render_image_views[i], vk_depth_image_views[i], vk_swapchain_image_views[i] };
			VkImageView vk_framebuffer_attach [] { vk_swapchain_image_views[i], vk_depth_image_views[i] };

			vk_framebuffers[i] = vk_dev.Framebuffer
			(
				vk_render_pass,
				// 3, vk_framebuffer_attach,
				2, vk_framebuffer_attach,
				800, 600,
				1
			);

			vk_submission_completed_fences[i] = vk_dev.Fence(VK_FENCE_CREATE_SIGNALED_BIT);

			VkSemaphoreTypeCreateInfo vk_semaphore_type_ci
			{
				VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
				nullptr,
				VK_SEMAPHORE_TYPE_BINARY,
				0
			};

			vk_submission_completed_semaphores[i] = vk_dev.Semaphore(0, &vk_semaphore_type_ci);
			vk_image_available_semaphores[i] = vk_dev.Semaphore(0, &vk_semaphore_type_ci);
		}



		// memory allocation

		vk_vertex_buffer = vk_dev.Buffer(vertices_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		VkMemoryRequirements vk_vertex_buffer_mem_reqs { vk_dev.MemReqs(vk_vertex_buffer) };

		uint64_t vk_vertex_buffer_mem_index { vk_dev.getMemTypeIndex(&vk_vertex_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };
		// cout << "MEM " << vk_vertex_buffer_mem_index << endl;

		VkDeviceMemory vk_vertex_buffer_mem { vk_dev.Mem(vk_vertex_buffer_mem_reqs.size, vk_vertex_buffer_mem_index) };

		vk_dev.bindMem(vk_vertex_buffer, vk_vertex_buffer_mem);

		void* vk_vertex_buffer_mem_addr { vk_dev.mapMem(vk_vertex_buffer_mem, 0, vertices_size, 0) };

		memcpy(vk_vertex_buffer_mem_addr, vertices, vertices_size);



		VkBuffer vk_uniform_buffer { vk_dev.Buffer(128, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE) };

		VkMemoryRequirements vk_uniform_buffer_mem_reqs { vk_dev.MemReqs(vk_uniform_buffer) };

		uint64_t vk_uniform_buffer_mem_index { vk_dev.getMemTypeIndex(&vk_uniform_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

		VkDeviceMemory vk_uniform_buffer_mem { vk_dev.Mem(vk_uniform_buffer_mem_reqs.size, vk_uniform_buffer_mem_index) };

		vk_dev.bindMem(vk_uniform_buffer, vk_uniform_buffer_mem);

		vk_uniform_buffer_mem_addr = vk_dev.mapMem(vk_uniform_buffer_mem, 0, 128, 0);

		memcpy(vk_uniform_buffer_mem_addr, &orbit, 128);



		// descriptors

		// VkDescriptorSetLayoutBinding vk_descr_set_layout_binding = { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr };
		VkDescriptorSetLayoutBinding vk_descr_set_layout_binding []
		{
			{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			{ 2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			{ 3, VK_DESCRIPTOR_TYPE_SAMPLER, 2, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
		};

		VkDescriptorSetLayout vk_descr_set_layout { vk_dev.DescrSetLayout(2, vk_descr_set_layout_binding) };



		VkDescriptorPoolSize vk_descr_pool_size { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 };

		VkDescriptorPool vk_descr_pool { vk_dev.DescrPool(2, 1, &vk_descr_pool_size) };

		// VkDescriptorPool vk_descr_pool
		// {
		// 	vk_dev.DescrPool
		// 	(
		// 		2,
		// 		0, nullptr
		// 	)
		// };

		VkDescriptorSetLayout lay [] { vk_descr_set_layout, vk_descr_set_layout };

		vk_descr_set = vk_dev.DescrSet(vk_descr_pool, 2, lay);



		VkDescriptorBufferInfo vk_descr_bi { vk_uniform_buffer, 0, VK_WHOLE_SIZE };

		// VkWriteDescriptorSet write_descr_set = WriteDescrSet(

		// 	vk_descr_set[0], 1, 0,
		// 	1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		// 	nullptr,
		// 	&vk_descr_bi,
		// 	nullptr
		// );

		VkWriteDescriptorSet write_descr_set []
		{
			WriteDescrSet
			(
				vk_descr_set[0], 0, 0,
				1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				nullptr,
				&vk_descr_bi,
				nullptr
			),

			WriteDescrSet
			(
				vk_descr_set[0], 0, 1,
				1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				nullptr,
				&vk_descr_bi,
				nullptr
			),

			WriteDescrSet
			(
				vk_descr_set[0], 1, 0,
				1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				nullptr,
				&vk_descr_bi,
				nullptr
			),

			WriteDescrSet
			(
				vk_descr_set[0], 1, 1,
				1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				nullptr,
				&vk_descr_bi,
				nullptr
			)

			// WriteDescrSet
			// (
			// 	vk_descr_set[0], 1, 0,
			// 	1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			// 	nullptr,
			// 	&vk_descr_bi,
			// 	nullptr
			// )
		};

		vk_dev.updateDescrSets(4, write_descr_set, 0, nullptr);



		// pipelines

		// default pipeline

		VkPipelineInputAssemblyStateCreateInfo vk_default_ppl_input_asm { PplInputAsm(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE) };

		VkPipelineTessellationStateCreateInfo vk_default_ppl_tess { PplTess(0, 0) };

		// flip vulkan viewport
		VkViewport viewport { 0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f };
		// VkViewport viewport = { 0.0f, 600.0f, 800.0f, -600.0f, 0.0f, 1.0f };
		VkRect2D scissor { { 0, 0 }, { 800, 600 } };

		VkPipelineViewportStateCreateInfo vk_default_ppl_view { PplView(1, &viewport, 1, &scissor) };

		VkPipelineMultisampleStateCreateInfo vk_default_ppl_sample { PplSample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE) };
		// VkPipelineMultisampleStateCreateInfo vk_default_ppl_sample { PplSample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE) };

		VkPipelineRasterizationStateCreateInfo vk_default_ppl_rast { PplRast(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f) };

		VkStencilOpState vk_default_ppl_stenc { VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0, 0, 0 };

		VkStencilOpState
			front {};
			front.failOp = VK_STENCIL_OP_KEEP;
			front.passOp = VK_STENCIL_OP_KEEP;
			front.compareOp = VK_COMPARE_OP_ALWAYS;

		VkStencilOpState
			back {};
			back.failOp = VK_STENCIL_OP_KEEP;
			back.passOp = VK_STENCIL_OP_KEEP;
			back.compareOp = VK_COMPARE_OP_ALWAYS;

		VkPipelineDepthStencilStateCreateInfo vk_default_ppl_depth_stenc { PplDepthStenc(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_FALSE, VK_FALSE, vk_default_ppl_stenc, vk_default_ppl_stenc, 0.0f, 1.0f) };

		// VkPipelineDepthStencilStateCreateInfo vk_default_ppl_depth_stenc { PplDepthStenc(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, { 0 }, { 0 }, 0.0f, 1.0f) };

		VkPipelineColorBlendAttachmentState vk_blend_attach
		{
			VK_FALSE,
			VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
			VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo vk_default_ppl_blend
		{
			PplBlend
			(
				VK_FALSE,
				VK_LOGIC_OP_CLEAR,
				1, &vk_blend_attach,
				0.0f, 0.0f, 0.0f, 0.0f
			)
		};

		VkPipelineDynamicStateCreateInfo vk_default_ppl_dyn { PplDyn(0, nullptr) };



		VkPipelineShaderStageCreateInfo vk_ppl_stages []
		{
			PplShader(VK_SHADER_STAGE_VERTEX_BIT, vk_dev.Shader(sizeof(vertex_shader_code_vulkan), vertex_shader_code_vulkan)),
			PplShader(VK_SHADER_STAGE_FRAGMENT_BIT, vk_dev.Shader(sizeof(fragment_shader_code_vulkan), fragment_shader_code_vulkan))
		};

		VkVertexInputBindingDescription vk_vertex_binding { 0, 12, VK_VERTEX_INPUT_RATE_VERTEX };
		VkVertexInputAttributeDescription vk_vertex_attr { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 };

		VkPipelineVertexInputStateCreateInfo vk_ppl_vertex { PplVertex(1, &vk_vertex_binding, 1, &vk_vertex_attr) };

		vk_ppl_layout = vk_dev.PplLayout(1, &vk_descr_set_layout);

		vk_ppl =
			vk_dev.PplG
			(
				2, vk_ppl_stages,
				&vk_ppl_vertex,
				&vk_default_ppl_input_asm,
				&vk_default_ppl_tess,
				&vk_default_ppl_view,
				&vk_default_ppl_rast,
				&vk_default_ppl_sample,
				&vk_default_ppl_depth_stenc,
				&vk_default_ppl_blend,
				&vk_default_ppl_dyn,
				vk_ppl_layout,
				vk_render_pass, 0
			);

		// vk_ppl_layout2 = vk_dev.PplLayout(1, &vk_descr_set_layout);

		vk_ppl2 =
			vk_dev.PplG
			(
				2, vk_ppl_stages,
				&vk_ppl_vertex,
				&vk_default_ppl_input_asm,
				&vk_default_ppl_tess,
				&vk_default_ppl_view,
				&vk_default_ppl_rast,
				&vk_default_ppl_sample,
				&vk_default_ppl_depth_stenc,
				&vk_default_ppl_blend,
				&vk_default_ppl_dyn,
				vk_ppl_layout,
				vk_render_pass, 0
			);



		// command buffers

		VkCommandPool vk_cmd_pool { vk_dev.CmdPool(vk_dev.graphics_queue_family_index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) };

		vk_cmd_buffers = vk_dev.CmdBuffer(vk_cmd_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, vk_swapchain_image_count);



		// VkCommandBufferBeginInfo vk_command_buffer_bi = CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

		clear_value[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clear_value[1].depthStencil = { 1.0f, 0 };

		vk_submit_i.resize(vk_swapchain_image_count);
		vk_present_i.resize(vk_swapchain_image_count);
		vk_image_indices.resize(vk_swapchain_image_count);
		vk_render_pass_bi.resize(vk_swapchain_image_count);

		static const VkPipelineStageFlags vk_wait_stages { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		for (uint64_t i = 0; i < vk_swapchain_image_count; ++i)
		{
			vk_submit_i[i] =
				SubmitI
				(
					1, &vk_image_available_semaphores[i], &vk_wait_stages,
					1, &vk_cmd_buffers[i],
					1, &vk_submission_completed_semaphores[i]
				);

			vk_present_i[i] =
				PresentI
				(
					1, &vk_submission_completed_semaphores[i],
					1, &vk_swapchain,
					nullptr
				);

			vk_render_pass_bi[i] = RenderPassBeginI(vk_render_pass, vk_framebuffers[i], { 0, 0, 800, 600 }, 2, clear_value);
		}



		loop_function = loop_function_VK;

		destroy_api_function = destroyVK;
	}
};
