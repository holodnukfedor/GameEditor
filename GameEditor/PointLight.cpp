#include "PointLight.h"

const float PointLight::SHADOW_NEAR_PLANE = 0.01f;

PointLight::PointLight()
{
  
}


PointLight::~PointLight()
{
}

void PointLight::Initialize(float linearAttenuation, float quadraticAttenuation, XMFLOAT3 position, XMFLOAT3 color, GameObject* parent, bool castShadows)
{
  SetParent(parent);
  SetColor(color);
  SetAttenuation(linearAttenuation, quadraticAttenuation);
  SetPosition(position.x, position.y, position.z);
  this->castShadows = castShadows;
}

void PointLight::SetAttenuation(float linearAttenuation, float quadraticAttenuation)
{
  m_linerarAttenuation = linearAttenuation;
  m_quadraticAttenuation = quadraticAttenuation;

  float maxColorPart = (std::max)({ m_color.x, m_color.y, m_color.z });
  float D = pow(m_linerarAttenuation, 2) - 4 * m_quadraticAttenuation * (1 - 256.0f * maxColorPart);

  if (D < 0)
  {
    this->SetScale(0.0f);
    return;
  }

  if (D == 0)
  {
    m_radius = -m_linerarAttenuation / (2 * m_quadraticAttenuation);
  }
  else if (D > 0)
  {
    float x1 = (-m_linerarAttenuation + sqrtf(D)) / (2 * m_quadraticAttenuation);
    float x2 = (-m_linerarAttenuation - sqrtf(D)) / (2 * m_quadraticAttenuation);
    m_radius = max(x1, x2);
  }
  m_radius *= 100.0f;
  this->SetScale(m_radius);

  RebuildPerspectiveValues();
}

void PointLight::SetPosition(float x, float y, float z)
{
  GameObject::SetPosition(x, y, z);
  XMFLOAT3 worldPosition = this->GetWorldPosition();
  RebuildCubeViewProjection(worldPosition);
}

void PointLight::RebuildCubeViewProjection(XMFLOAT3 worldPosition)
{
  XMMATRIX lightProjection, positionMatrix, spotView, toShadow;

  positionMatrix = XMMatrixTranslation(-worldPosition.x, -worldPosition.y, -worldPosition.z);
  lightProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, SHADOW_NEAR_PLANE, m_radius * 1000);

  // Cube +X
  spotView = XMMatrixRotationY(XM_PI + XM_PIDIV2);
  toShadow = positionMatrix * spotView * lightProjection;
  m_cubeViewProjection[0] = XMMatrixTranspose(toShadow);

  // Cube -X
  spotView = XMMatrixRotationY(XM_PIDIV2);
  toShadow = positionMatrix * spotView * lightProjection;
  m_cubeViewProjection[1] = XMMatrixTranspose(toShadow);

  // Cube +Y
  spotView = XMMatrixRotationX(XM_PIDIV2);
  toShadow = positionMatrix * spotView * lightProjection;
  m_cubeViewProjection[2] = XMMatrixTranspose(toShadow);

  // Cube -Y
  spotView = XMMatrixRotationX(XM_PI + XM_PIDIV2);
  toShadow = positionMatrix * spotView * lightProjection;
  m_cubeViewProjection[3] = XMMatrixTranspose(toShadow);

  // Cube +Z
  toShadow = positionMatrix * lightProjection;
  m_cubeViewProjection[4] = XMMatrixTranspose(toShadow);

  // Cube -Z
  spotView = XMMatrixRotationY(XM_PI);
  toShadow = positionMatrix * spotView * lightProjection;
  m_cubeViewProjection[5] = XMMatrixTranspose(toShadow);

  m_oldWorldPosition = worldPosition;
}

XMMATRIX* PointLight::GetCubeViewProjection()
{
  XMFLOAT3 worldPosition = this->GetWorldPosition();

  float precision = 0.001;

  if (abs(m_oldWorldPosition.x - worldPosition.x) > precision || abs(m_oldWorldPosition.y - worldPosition.y) > precision != abs(m_oldWorldPosition.z - worldPosition.z) > precision)
  {
    RebuildCubeViewProjection(worldPosition);
  }

  return m_cubeViewProjection;
}

void PointLight::RebuildPerspectiveValues()
{
  XMMATRIX lightProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, SHADOW_NEAR_PLANE, m_radius);
  XMFLOAT4X4 lightProjectionReadable;

  XMStoreFloat4x4(&lightProjectionReadable, lightProjection);
  m_lightPerspectiveValues = XMFLOAT2(lightProjectionReadable._33, lightProjectionReadable._43);
}

XMFLOAT2 PointLight::GetLightPerspectiveValues()
{
  return m_lightPerspectiveValues;
}