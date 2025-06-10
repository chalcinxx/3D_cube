#include "CX/Math/Constants.hpp"
#include "CX/EventHandler/EventHandler.hpp"
#include "CX/Plane/Plane.hpp"

int main()
{
   // Set windows settings
   sf::RenderWindow window (sf::VideoMode().getDesktopMode(), "3D Cube", sf::Style::Fullscreen);
   window.setVerticalSyncEnabled(true);
   window.setFramerateLimit(60);

   cx::EventHandler event (window);

   cx::Plane faces[6];                    // Cube faces
   cx::Vec3f rotation;                    // Cube rotation
   cx::Vec3f offset;                      // Cube offset (position in 3d space)
   cx::Vec2f center = event.get_center(); // Cube position 

   // Set a different color for all faces. I don't recommend as the faces won't
   // get rendered correctly. I tried multiple solutions, but they failed when
   // the cube started moving. My only option was rasterization, but that would
   // be slow and ugly.
   /* */
   // faces[0].set_color(cx::Color::red());   // Front
   // faces[1].set_color(cx::Color::red());   // Back
   // faces[2].set_color(cx::Color::green()); // Right
   // faces[3].set_color(cx::Color::green()); // Left
   // faces[4].set_color(cx::Color::blue());  // Top
   // faces[5].set_color(cx::Color::blue());  // Bottom
   /* */
   // Or alternatively set one color to all textures
   /* */
   for (auto& f : faces)
      f.set_color(cx::Color::white());
   /* */

   constexpr float size = 100.f;                            // Size of the cube
   constexpr float pi = cx::Constants<float>::pi;           // 180 degrees in radians
   constexpr float half_pi = cx::Constants<float>::half_pi; // 90 degrees in radians

   constexpr cx::Vec3f face_offsets[6] = {
      {0.f, 0.f, size},  // Front
      {0.f, 0.f, -size}, // Back
      {-size, 0.f, 0.f}, // Right
      {size, 0.f, 0.f},  // Left
      {0.f, -size, 0.f}, // Top
      {0.f, size, 0.f}   // Bottom
   }; 

   constexpr cx::Vec3f face_rotations[6] {
      {0.f, 0.f, 0.f},      // Front
      {0.f, pi, 0.f},       // Back
      {0.f, -half_pi, 0.f}, // Right
      {0.f, half_pi, 0.f},  // Left
      {-half_pi, 0.f, 0.f}, // Top 
      {half_pi, 0.f, 0.f}   // Bottom
   };

   constexpr cx::Vec3f face_corners[4] {
      {-size, -size, 0.f}, // Top-left
      {size, -size, 0.f},  // Top-right
      {size, size, 0.f},   // Bottom-right
      {-size, size, 0.f}   // Bottom-left
   };

   while (window.isOpen())
   {
      event.update();

      // Quit the demo
      if (event.is_released(cx::Key::esc))
         window.close();

      // Rotate the cube using keys
      rotation.x += event.get_axis(cx::Key::w, cx::Key::s) * event.get_dt();
      rotation.y += event.get_axis(cx::Key::d, cx::Key::a) * event.get_dt();
      rotation.z += event.get_axis(cx::Key::e, cx::Key::q) * event.get_dt();

      // Move the cube in 3d space
      offset.x += event.get_axis(cx::Key::j, cx::Key::l) * 10.f;
      offset.y += event.get_axis(cx::Key::i, cx::Key::k) * 10.f;
      offset.z += event.get_axis(cx::Key::u, cx::Key::o) * 10.f;

      // Rotate and move the cube using mouse
      if (event.is_down(cx::Mouse::left))
      {
         event.set_mouse_mode(cx::MouseMode::locked);
         rotation.x += -event.get_mouse_delta().y * event.get_dt() * .33f;
         rotation.y += event.get_mouse_delta().x * event.get_dt() * .33f;
      }
      else if (event.is_down(cx::Mouse::middle))
      {
         event.set_mouse_mode(cx::MouseMode::locked);
         offset.x += event.get_mouse_delta().x * event.get_dt() * 15.f;
         offset.y += event.get_mouse_delta().y * event.get_dt() * 15.f;
      }
      else event.set_mouse_mode(cx::MouseMode::free);

      // Update the faces
      for (size_t face = 0; face < 6; ++face)
      {
         cx::Vec3f pos = face_offsets[face].rotate(rotation); 
         cx::Vec2f projected = pos.project(500.f);
         faces[face].set_center(center + projected);
         faces[face].set_3d_rotation(face_rotations[face] + rotation);
         faces[face].set_3d_offset_z(pos.z);

         for (size_t i = 0; i < 4; ++i)
         {
            cx::Vec3f transformed = face_corners[i].rotate(face_rotations[face]) + face_offsets[face];
            transformed = transformed.rotate(rotation) + offset;

            faces[face].get_vertex(i).position = transformed.project(500.f) + center;
         }
      }

      // Render the faces
      window.clear();
      for (const auto& f : faces)
         f.render(window);
      window.display();
   }
}
