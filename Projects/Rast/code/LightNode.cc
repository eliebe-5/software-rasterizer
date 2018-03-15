#include "Projects/Rast/code/LightNode.h" 

void LightNode::applyMat()
{
	shader->setUniVec("lightPos", getPosition());
	shader->setUniVec("specColor", getColor());
}
