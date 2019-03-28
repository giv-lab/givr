//------------------------------------------------------------------------------
// A simple example showing how to use the triangle geometry
//------------------------------------------------------------------------------
#include "givr.h"
#include <functional>
#include <glm/gtc/matrix_transform.hpp>

#include "io.h"
#include "turntable_controls.h"

#include "grid2.h"

using namespace glm;
using namespace givr::camera;
using namespace givr::geometry;
using namespace givr::style;
using namespace givr::style;
using PrimitiveType = givr::PrimitiveType;

using givr::mat4f;
using givr::vec3f;

CustomGeometry<PrimitiveType::TRIANGLES> heightMapMesh;
geometry::Grid2 heightMap;
float g_time = 0.f;

void fill(geometry::Grid2 &grid,  // grid
          float xMin, float xMax, // domain x
          float yMin, float yMax, // domain y
          std::function<float(float, float)> const &func) {

  auto xSamples = grid.width();
  auto ySamples = grid.height();

  auto xDelta = (xMax - xMin) / xSamples;
  auto yDelta = (yMax - yMin) / ySamples;
  for (int h = 0; h < ySamples; ++h) {
    for (int w = 0; w < xSamples; ++w) {
      float x = xMin + w * xDelta;
      float y = yMin + h * yDelta;

      grid(w, h) = func(x, y);
    }
  }
}

void animate(float time) {

  auto func = [time](float x, float y) {
    constexpr float amp = 0.5f;
    return amp * std::sin((x * x + y * y) + time);
  };

  fill(heightMap, -M_PI * 2, M_PI * 2, // x
       -M_PI * 2, M_PI * 2,            // y
       func);
}

void loadHeightMapMesh(CustomGeometry<PrimitiveType::TRIANGLES> &mesh,
                       geometry::Grid2 const &grid, //
                       float xDelta = 0.1f, float yDelta = 0.1f) {
  mesh.vertices.clear();

  auto width = grid.width();
  auto height = grid.height();

  auto origin = givr::vec3f(-width * 0.5f * xDelta,  //
                            -height * 0.5f * xDelta, //
                            0.f);

  auto addTriangle = [&](vec3f const &p1, vec3f const &p2, vec3f const &p3) {
    mesh.vertices.push_back(p1);
    mesh.vertices.push_back(p2);
    mesh.vertices.push_back(p3);
  };

  for (int w = 0; w < width - 1; ++w) {
    for (int h = 0; h < height - 1; ++h) {
      //	 	(w,h) a - b
      // 					| \ |
      // 					c - d
      auto x = w * xDelta;
      auto y = h * yDelta;

      auto a = origin + vec3f(x, y, heightMap(w, h));
      auto b = origin + vec3f(x + xDelta, y, heightMap(w + 1, h));
      auto c = origin + vec3f(x, y + yDelta, heightMap(w, h + 1));
      auto d = origin + vec3f(x + xDelta, y + yDelta, heightMap(w + 1, h + 1));

      // counter-clockwise orientation
      addTriangle(a, c, d);
      addTriangle(a, d, b);
    }
  }
}

void init() {
  heightMapMesh = CustomGeometry<PrimitiveType::TRIANGLES>();
  heightMap = geometry::Grid2(1000, 1000);
  g_time = 0.f;

  animate(g_time);
  loadHeightMapMesh(heightMapMesh, heightMap);
}

int main(void) {
  io::GLFWContext windows;
  auto window =
      windows.create(io::Window::dimensions{800, 800}, "Height Map example");
  window.enableVsync(true);

  auto view = View(TurnTable(), Perspective());
  TurnTableControls controls(window, view.camera);

  init();
  auto phongStyle =
      givr::style::Phong(Colour(1., 0., 0.1529), LightPosition(100, 100, 100), GenerateNormals(true));

  auto heightMapBuffer = givr::createRenderable(heightMapMesh, phongStyle);

  glClearColor(1.f, 1.f, 1.f, 1.f);
  float u = 0.;
  window.run([&](float frameTime) {
    view.projection.updateAspectRatio(window.width(), window.height());
    g_time += 0.1f;

    animate(g_time);
    loadHeightMapMesh(heightMapMesh, heightMap);

    updateRenderable(heightMapMesh, phongStyle, heightMapBuffer);
    draw(heightMapBuffer, view,
         rotate(mat4f(1.f), (float(M_PI) / 2.f), vec3f(1.f, 0.f, 0.f)));
  });
  exit(EXIT_SUCCESS);
}
