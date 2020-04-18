#pragma once
#include <Optimus/Graphics/Renderer.h>

class MainRenderer : public OP::Renderer
{
public:
		 
	MainRenderer();

	void Start()override;
	void Update()override;

};
