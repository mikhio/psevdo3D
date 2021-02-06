#include <SFML/Graphics.hpp>
#include "include/settings.hpp"
#include <iostream>
#include <cmath>

struct Person {
    float x;
    float y;
    float angle;
    float speed;
    float fov;
    int raysNum;
    float maxDepth;
    float rayStep;
    float distToDis;
};

void rayCasting(Person *pl, char map[MAP_HEIGHT][MAP_WIDTH+1], sf::RenderWindow *win);


float getDist(sf::Vector2f a, sf::Vector2f b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}


void drawPlayer(sf::RenderWindow *win, Person *pl, char map[MAP_HEIGHT][MAP_WIDTH+1]) {
    // float rad = 10.f;

    // sf::CircleShape shape(rad);
    // shape.setOrigin(rad, rad);
    // shape.setPosition(pl->x * MAP_SCALE, pl->y * MAP_SCALE);
    // shape.setFillColor(sf::Color(200, 0, 0));

    rayCasting(pl, map, win);

    // for (int i = 0; i < pl->raysNum; ++i) {
    //     // std::cout << dists[i].x << ' ' << dists[i].y << '\n';
    //     // sf::Vertex line[] = {
    //     //     sf::Vertex(shape.getPosition()),
    //     //     sf::Vertex(sf::Vector2f(dists[i].x * MAP_SCALE, dists[i].y * MAP_SCALE))
    //     // };
    //     // line[0].color = sf::Color(255, 0, 0);
    //     // line[1].color = sf::Color(255, 0, 0);

    //     // win->draw(line, 2, sf::Lines);
    //     // float dist = getDist(sf::Vector2f(pl->x, pl->y), dists[i]);
    // }


    // sf::RectangleShape lineShape(sf::Vector2f(100.f, 1.f));
    // lineShape.setPosition(sf::Vector2f(pl->x*MAP_SCALE, pl->y*MAP_SCALE-0.5f));
    // lineShape.setFillColor(sf::Color(0, 0, 255));
    // lineShape.setRotation(pl->angle);

    // win->draw(lineShape);
    // win->draw(shape);
}


void rayCasting(Person *pl, char map[MAP_HEIGHT][MAP_WIDTH+1], sf::RenderWindow *win) {
    float curAng = pl->angle - pl->fov/2;
    for (int i = 0; i < pl->raysNum; ++i) {
        float xo = pl->x, yo = pl->y;
        float d;
        for (d = pl->rayStep; d < pl->maxDepth; d+=pl->rayStep) {
            xo = pl->x + d * cos(curAng * PI/180);
            yo = pl->y + d * sin(curAng * PI/180);

            if (map[int(yo)][int(xo)] == '1') {
                break;
            }
        }

        d *= cos((pl->angle - curAng) * PI/180);

        float h = pl->distToDis * WALL_HEIGHT / d;
        float colorCof = 255 / (1 + d * 0.18);

        // std::cout << colorCof << '\n';

        sf::RectangleShape colLine(sf::Vector2f(WINDOW_WIDTH / pl->raysNum, h));
        colLine.setOrigin(sf::Vector2f(0.f, h / 2.f));
        colLine.setPosition(sf::Vector2f(WINDOW_WIDTH / pl->raysNum * i, WINDOW_HEIGHT / 2));
        colLine.setFillColor(sf::Color(colorCof / 2, colorCof, colorCof / 3));

        win->draw(colLine);

        curAng += pl->fov/pl->raysNum;
    }
}


int main() {
    // Create window and set FPS
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Test");
    window.setFramerateLimit(60);


    Person player;
    player.x = 2.5f;
    player.y = 1.5f;
    player.angle = 0.f;
    player.speed = 0.05f;
    player.fov = 60.f;
    player.raysNum = 300.f;
    player.maxDepth = 8.f;
    player.rayStep = 0.01f;
    player.distToDis = player.raysNum / (2 * tan(player.fov/2 * PI/180));

    // Cretae map
    char map[MAP_HEIGHT][MAP_WIDTH+1] = {
        "111111111111111111111111111111111111111111111111111111111111",
        "1........1......1.........1..........1.....................1",
        "111111.1111111.1111..111111.11...............1........111..1",
        "1...1........1.1..1..1.1.1..11..................11.......1.1",
        "1.1.1...1111.1.1..1..1.1.....1................1........11..1",
        "1.1.11.11....1.1.......1.11111.....1.......................1",
        "1.1...............1111.1.1...1.............................1",
        "111..11111111.111.111111.111.1............1......1.........1",
        "1...........1..1.............1.............................1",
        "111111111111111111111111111111111111111111111111111111111111"
    };

    while (window.isOpen()) {
        // Check event
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.x += player.speed * cos(player.angle * PI/180);
            player.y += player.speed * sin(player.angle * PI/180);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.x -= player.speed * cos(player.angle * PI/180);
            player.y -= player.speed * sin(player.angle * PI/180);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.angle += 3.f;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.angle -= 3.f;
        }

        // Clearing, updating and drawing
        window.clear(sf::Color(36, 197, 237));

        // for (int y = 0; y < MAP_HEIGHT; ++y) {
        //     for (int x = 0; x < MAP_HEIGHT; ++x) {
        //         if (map[y][x] == '1') {
        //             sf::RectangleShape shape(sf::Vector2f(MAP_SCALE, MAP_SCALE));
        //             shape.setPosition(sf::Vector2f(x*MAP_SCALE, y*MAP_SCALE));
        //             shape.setFillColor(sf::Color(0, 0, 0));

        //             window.draw(shape);
        //         }
        //     }
        // }

        sf::RectangleShape floor(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT/2));
        floor.setPosition(sf::Vector2f(0.f, WINDOW_HEIGHT/2));
        floor.setFillColor(sf::Color(50, 50, 50));

        window.draw(floor);

        drawPlayer(&window, &player, map);

        window.display();

    }

    return 0;
}