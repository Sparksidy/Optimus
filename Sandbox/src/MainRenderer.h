#pragma once
#include <Optimus/Graphics/Core/Renderer.h>
#include <Optimus/Log.h>


using namespace OP;

class MainRenderer : public Renderer
{
public:

	MainRenderer();
	~MainRenderer() 
	{
		OP_CORE_INFO("Destroying Main Renderer");
	}

	void Start()override;
	void Update()override;
};