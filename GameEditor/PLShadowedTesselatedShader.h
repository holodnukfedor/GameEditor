#pragma once

#include "IShader.h"
#include "PointLightDefferedParameters.h"

class PLShadowedTesselatedShader : public IShader
{
protected:
  struct LightProjectionBuffer
  {
    XMMATRIX LightProjection;
  };
  struct WorldCoordsUnpackBuffer
  {
    XMVECTOR perspectiveValues;
    XMMATRIX viewMatrixInverse;
  };
  struct PointLightBuffer
  {
    XMFLOAT3 color;
    float linearAttenuation;
    XMFLOAT3 position;
    float quadraticAttenuation;
    XMFLOAT2 lightPerspectiveValues;
    XMFLOAT2 padding;
  };
protected:
  ID3D11Buffer* m_lightProjectionBuffer;
  ID3D11Buffer* m_worldCoordsUnpackBuffer;
  ID3D11Buffer* m_pointLightBuffer;
  ID3D11SamplerState* m_sampleState;
protected:
  virtual void InitializeShader(ID3D11Device* device, HWND hwnd, const std::wstring& vsFilename, const std::wstring& gsFilename, const std::wstring& hlFilename, const std::wstring& dmShaderFileName, const std::wstring& psFilename) override;
  virtual void ShutdownShader() override;
  void SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX lightProjection, XMFLOAT3 pointLightColor, XMFLOAT3 pointLightPosition, float linearAttenuation, float quadraticAttenuation, XMVECTOR perspectiveValues, XMMATRIX viewMatrixInv, XMFLOAT2 lightPerspectiveValues);
public:
  PLShadowedTesselatedShader();
  virtual ~PLShadowedTesselatedShader();
  virtual void EnableShader(ID3D11DeviceContext* deviceContext);
  virtual void Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, const IMaterial* material, LightininigSystem* lightining, const XMFLOAT3& cameraPosition) override;
};

