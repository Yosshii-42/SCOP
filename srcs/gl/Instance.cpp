#include "gl/Instance.hpp"
#include "Common.hpp"

float randomFloat(float min, float max)
{
  float r = static_cast<float>(rand()) /
              static_cast<float>(RAND_MAX);
  return (min + (max - min) * r);
}

void  Instance::setInstance(std::vector<Instance>& instances)
{
  Instance  instance;
  Vec3      axis;

  instance.position.x = randomFloat(-2.0f, 2.0f);
  instance.position.y = randomFloat(-2.0f, 2.0f);
  instance.position.z = randomFloat(-2.0f, 2.0f);

  axis.x = randomFloat(-2.0f, 2.0f);
  axis.y = randomFloat(-2.0f, 2.0f);
  axis.z = randomFloat(-2.0f, 2.0f);
  if (axis.length() < Common::VECTOR_EPSILION)
    axis = Vec3(0.0f, 1.0f, 0.0f);
  instance.rotationAxis = axis.normalize();

  instance.scale = randomFloat(0.2f, 0.7f);
  instance.rotationSpeed = randomFloat(0.2f, 1.0f);

  if (instances.empty())
  {
    instance.position = Vec3(0.0f, 0.0f, 0.0f);
    instance.scale = 1.0f;
  }

  instances.push_back(instance);
}
