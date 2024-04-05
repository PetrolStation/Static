#pragma once

#include "Core/Aliases.h"

#include <memory>

#include "Core/DebugTools.h"
#include <Core/Components/Transform.h>
#include <Core/Components/Mesh.h>
#include <Core/Components/Camera.h>
#include <Freetype/Renderer/Text.h>
#include "Core/Renderer/RendererResourceCreator.h"
#include <Core/Components/Vertex.h>

// abstract classes
#include <Core/Renderer/Framebuffer.h>

#include "Core/Files.h"

namespace PetrolEngine {

    struct RendererResources{
        UnorderedMap<String, Ref<Shader>> shaders;
        UnorderedMap<String, Ref<Texture>> fonts;
    };

    class Renderer {
    public:
        static UnorderedMap<RRC*, RendererResources*> rendererResourcesCache;
        static  RendererResources* rendererResources;

        static void setContext(RRC& context){
            creator = &context;
            renderer = creator->newRenderer();
            auto x = rendererResourcesCache.find(creator);

            if(x == rendererResourcesCache.end())
                rendererResourcesCache[creator] = new RendererResources();

            rendererResources = rendererResourcesCache[creator];
        }


        // framebuffer
        static     Framebuffer*    newFramebuffer(const FramebufferSpecification& spec) { LOG_FUNCTION(); return         creator->newFramebuffer(spec) ; }
        static Ref<Framebuffer> createFramebuffer(const FramebufferSpecification& spec) { LOG_FUNCTION(); return Ref<Framebuffer>(newFramebuffer(spec)); }

        // texture
        static Texture* newTexture(const String& path) { LOG_FUNCTION(); return creator->newTexture(Image(path)); }
        static Texture* newTexture(const Image& image) { LOG_FUNCTION(); return creator->newTexture(image); }
        static Texture* newTexture(int width, int height, TextureFormat format, TextureType type = TextureType::Texture2D) { LOG_FUNCTION(); return creator->newTexture(width, height, format, type); }

        static Ref<Texture> createTexture(const String& path){ LOG_FUNCTION(); return Ref<Texture>(newTexture(path)); }
        static Ref<Texture> createTexture(const Image& image){ LOG_FUNCTION(); return Ref<Texture>(newTexture(image)); }
        static Ref<Texture> createTexture(int width, int height, TextureFormat format, TextureType type = TextureType::Texture2D){ LOG_FUNCTION(); return Ref<Texture>(newTexture(width, height, format, type)); }

        // Vertex array
        static     VertexArray*    newVertexArray() { LOG_FUNCTION(); return         creator->newVertexArray() ; }
        static Ref<VertexArray> createVertexArray() { LOG_FUNCTION(); return Ref<VertexArray>(newVertexArray()); }

        // Vertex buffer
        static     VertexBuffer*    newVertexBuffer(VertexLayout layout) { LOG_FUNCTION(); return          creator->newVertexBuffer(layout) ; }
        static Ref<VertexBuffer> createVertexBuffer(VertexLayout layout) { LOG_FUNCTION(); return Ref<VertexBuffer>(newVertexBuffer(layout)); }

        static     VertexBuffer*    newVertexBuffer(VertexLayout layout, const void* data, int64 size) { LOG_FUNCTION(); return          creator->newVertexBuffer(layout, data, size) ; }
        static Ref<VertexBuffer> createVertexBuffer(VertexLayout layout, const void* data, int64 size) { LOG_FUNCTION(); return Ref<VertexBuffer>(newVertexBuffer(layout, data, size)); }

        // index buffer
        static     IndexBuffer*    newIndexBuffer(const void* data, int64 size) { LOG_FUNCTION(); return         creator->newIndexBuffer(data, size) ; }
        static Ref<IndexBuffer> createIndexBuffer(const void* data, int64 size) { LOG_FUNCTION(); return Ref<IndexBuffer>(newIndexBuffer(data, size)); }

        static     IndexBuffer*    newIndexBuffer() { LOG_FUNCTION(); return         creator->newIndexBuffer() ; }
        static Ref<IndexBuffer> createIndexBuffer() { LOG_FUNCTION(); return Ref<IndexBuffer>(newIndexBuffer()); }

        // graphics context
        static     GraphicsContext*    newGraphicsContext(){ LOG_FUNCTION(); return             creator->newGraphicsContext() ; }
        static Ref<GraphicsContext> createGraphicsContext(){ LOG_FUNCTION(); return Ref<GraphicsContext>(newGraphicsContext()); }

        // shader
        static Shader* newShader(const String&           name,
                                 const String&   vertexShader,
                                 const String& fragmentShader,
                                 const String& geometryShader  ) {
            return creator->newShader(name, vertexShader, fragmentShader, geometryShader);
        }
        static Ref<Shader> createShader(const String&        name,
                                        const String&   vertexSrc,
                                        const String& fragmentSrc,
                                        const String& geometrySrc ){
            return Ref<Shader>(newShader(name, vertexSrc, fragmentSrc, geometrySrc));
        }


        static Ref<Shader> loadShader(const String& filePath);
        static Ref<Shader> loadShader(const String& name       ,
                                      const String& vertexSrc  ,
                                      const String& fragmentSrc,
                                      const String& geometrySrc = "" );

        static int  init    (bool debug) { LOG_FUNCTION(); return renderer->init(debug); };
        static void setViewport(int x, int y, int width, int height) { LOG_FUNCTION(); return renderer->setViewport(x, y, width, height); };
		static void drawQuad2D(const Texture* texture, const Transform* transform, Shader* shader, const Camera* camera, glm::vec4 texCoords = {0,0,1,1}){ LOG_FUNCTION();
		    renderer->drawQuad2D(texture, transform, shader, camera, texCoords);
		}
        static void renderMesh (Mesh& mesh, Transform& transform) { LOG_FUNCTION();
            auto vao = mesh.getVertexArray();
            Vector<const Texture*> textures;

            textures.reserve(mesh.meshRenderer->material.textures.size());
            for(auto& texture : mesh.meshRenderer->material.textures)
                textures.push_back(texture.get());

            return renderer->renderMesh(vao.get(), transform, textures, mesh.meshRenderer->material.shader.get(), camera);
        }

        static void renderText(const String& text, const Transform& transform, const String& font) { LOG_FUNCTION();
            Material fontMaterial;
            Text::FontAtlas* atlas = &Text::getAtlas(font);

            auto texture = rendererResources->fonts.find(font);

            if(texture == rendererResources->fonts.end()){
                rendererResources->fonts[font] = Renderer::createTexture(
                        atlas->atlas->getWidth(),
                        atlas->atlas->getHeight(),
                        TextureFormat::RED,
                        TextureType::Texture2D
                );
                rendererResources->fonts[font]->updateTextureImage(atlas->atlas->getData());
                //rendererResources->fonts[font] = Renderer::createTexture("resources/enemy.png");
            }

            /*auto shader = Renderer::loadShader( "textShader",
                    ReadFile("resources/textShader.vert"),
                    ReadFile("resources/textShader.frag")
            );*/
            auto shader = Renderer::loadShader( "textShader",
          ReadFile("Resources/Shaders/textShader.vert"),
          ReadFile("Resources/Shaders/textShader.frag")
            );
            return renderer->renderText(text, transform, rendererResources->fonts[font].get(), atlas, shader.get(), camera);
        };
        
        static void draw() {renderer->draw();}
        static void clear() { LOG_FUNCTION(); return renderer->clear(); };
        static void getDeviceConstantValue(DeviceConstant deviceConstant, void* outputBuffer) { LOG_FUNCTION(); return renderer->getDeviceConstantValue(deviceConstant, outputBuffer); };
        static void renderFramebuffer(const Ref<Framebuffer> framebuffer, const Ref<Shader> shader) { LOG_FUNCTION();
            const Vector<Vertex> planeVertices = {
                    // pos        // tex
                    Vertex({-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),
                    Vertex({ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}),
                    Vertex({-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}),

                    Vertex({-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),
                    Vertex({ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}),
                    Vertex({ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f})
            };
        };
        static void setCamera(const Camera* cam) { camera = cam; }
        // static void drawQuad2D(Material material, Transform transform) { return renderer->drawQuad2D(material, transform); };

        ~Renderer() = default;

        static RRC* getCreator() { return creator; }

    private:
        static const Camera* camera;
        static RendererAPI* renderer;
        static RRC* creator;
    };
}
