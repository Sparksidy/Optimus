#pragma once
#include <Optimus/Graphics/Renderer.h>
#include <Optimus/Log.h>

using namespace OP;

class MainRenderer : public Renderer
{
public:

	MainRenderer();
	~MainRenderer() {}

	void Start()override;
	void Update()override;

};