
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
#include "main.hpp"


std::array<double, 2> fixPosition(std::array<double, 2> coordinates, sf::RenderWindow &window)
{
    double pixelLength = pow(10, 2);
    coordinates[0] /= pixelLength;
    coordinates[1] /= pixelLength;
    return std::array<double, 2> {coordinates[0], window.getSize().y-coordinates[1]};
}
int frames = 0;
int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(2560, 1600), "AstroSim");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // run the program as long as the window is open

    Renderer renderer(0);
    Matter planet1(pow(10,1), 3, {30000, 110000}, {0,50});
    Node<Matter> node1(planet1, NULL, NULL);
    renderer.addMatter(&node1);
    Matter planet2(pow(10,18), 11, {40000, 100000}, {0,0});
    Node<Matter> node2(planet2, NULL, NULL);
    renderer.addMatter(&node2);
    // Matter planet3(pow(10,1), 5, {50000, 100000}, {0,-30});
    // Node<Matter> node3(planet3, NULL, NULL);
    // renderer.addMatter(&node3);
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

//         clear the window with black color
        window.clear();

//         draw everything here...
//         window.draw(...);
        
//        sf::Texture bg;
//        bg.loadFromImage(canvas);
//        sf::Sprite backgroundSprite;
//        backgroundSprite.setTexture(bg, true);
//
        renderer.updateScene();
        renderer.traceObjects(window, pow(10,2));
        
        // y = 2x
//        for (int x = 0; x < size.x; x++)
//        {
////            std::cout << x << "\n";
//            int A = -0.5, B = 1.4, C = -1.3, D = 690, E = -0.3, F = 0;
//            int y1 = -(sqrt(pow((B*x+E),2)-4*C*(x*(A*x+D)+F))+ B*x + E)/(2*C);
//            int y2 = (sqrt(pow((B*x+E),2)-4*C*(x*(A*x+D)+F))+ B*x + E)/(2*C);
//            int y3;
//            if ((B*x+E) != 0)
//            {
//                y3 = -(x*(A*x+D)+F)/(B*x+E);
//            }
//            else
//            {
//                y3 = 20;
//            }
//            if (y1 < size.y)
//            {
//                graph.setPixel(x, y1, sf::Color(255, 255, 255));
//            }
//            if (y2 < size.y)
//            {
//                graph.setPixel(x, y2, sf::Color(255, 255, 255));
//            }
//            if (y3 < size.y)
//            {
//                graph.setPixel(x, y3, sf::Color(255, 255, 255));
//            }
//        }
//        sf::Texture texture;
//        texture.loadFromImage(graph);
//        sf::Sprite sprite;
//        sprite.setTexture(texture, true);
        if (frames % 700 == 0)
        {
            planet1.history.push_back(planet1.position);
        }
        Node<Matter> * node = &renderer.matter;
        window.draw(backgroundSprite);
        while (true)
        {
            std::array<double, 2> fixedPosition = fixPosition(node->value->position, window);
            sf::CircleShape shape (node->value->radius);
            shape.setPosition(fixedPosition[0], fixedPosition[1]);
            window.draw(shape);
            if (node->next != NULL)
            {
                node = node->next;
            }
            else
            {
                break;
            }
        }
        window.display();
        frames++;
    }
    return EXIT_SUCCESS;
}
