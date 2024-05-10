#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace VX
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top); // orthographic matrix, the bounce, optional: near, far
        virtual ~OrthographicCamera() = default;
        
        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

        // currently just for z-axis
        float GetRotation() const { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
        
        // settings
        // set Y-Flip as GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted. In vulkan, we flip it back
        void SetYFlip(bool isYFlip = true);

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        
        virtual void Update() = 0;
        static OrthographicCamera* Create(float left, float right, float bottom, float top);
        
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix; // a cache

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Rotation = 0.0f;
        
        bool m_isYFlip = true;
    };
}
