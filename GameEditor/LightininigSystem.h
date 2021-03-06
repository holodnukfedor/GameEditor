#pragma once

#include <directxmath.h>
#include <vector>

class Logger;
class PointLight;

using namespace DirectX;

class LightininigSystem
{
protected:
  XMVECTOR m_ambientColor;
  XMVECTOR m_directLightColor;
  XMFLOAT3 m_directLightDirection;
  std::vector<PointLight*> m_pointLightsNonCastShadows;
  std::vector<PointLight*> m_pointLightsCastShadows;
  bool m_castShadowRenderFlag = false;
  int m_pointLightToRenderIndex = 0;
public:
  void SetAmbientColor(float red, float green, float blue, float alpha) { m_ambientColor = XMVectorSet(red, green, blue, alpha); }
  void SetDirectLightColor(float red, float green, float blue, float alpha) { m_directLightColor = XMVectorSet(red, green, blue, alpha); }
  void SetDirectLightDirection(float x, float y, float z)  { m_directLightDirection = XMFLOAT3(x, y, z); }
  XMVECTOR GetAmbientColor() const { return m_ambientColor; }
  XMVECTOR GetDirectLightColor() const { return m_directLightColor; }
  XMFLOAT3 GetDirectLightDirection() const { return m_directLightDirection; }
  void AddPointLight(PointLight* pointLight);
  const std::vector<PointLight*>* GetPointLightsNonCastShadows() const { return &m_pointLightsNonCastShadows; }
  const std::vector<PointLight*>* GetPointLightsCastShadows() const { return &m_pointLightsCastShadows; }
  void ClearLights();
  LightininigSystem() = default;
  virtual ~LightininigSystem() = default;
  void SetPointLightToRenderSelector(bool castShadow, int index);
  PointLight* GetPointLightToRender() const;
};

