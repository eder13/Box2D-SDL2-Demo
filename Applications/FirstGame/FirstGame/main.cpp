//-----------------------------------------------------------------------------
///
/// A simple Demo of using Box2D with SDL2
///
/// @param argc not used
/// @param arv not used
///
///
/// @return EXIT_SUCCESS
//

#include <iomanip>
#include <Box2D/Box2D.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

b2World* world;

const int MET2PIX = 80; // 640 / 80 = 8

const int WIDTH = 640;
const int HEIGHT = 480;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;

using namespace std;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;
    
    cout << "Width of the Screen: " << Width << endl;
    cout << "Height of the Screen: " << Height << endl;
    
    SDL_Window *window = SDL_CreateWindow("FirstGame", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    world = new b2World(b2Vec2(0.0f, 9.81f)); // new b2World(b2Vec2(0.0f, 9.81f));
    
    // cartesian origin
    float ground_x = 0.0f;
    float ground_y = 0.0f;
    
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = 2.0;
    
    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3;
    endpoint.y = 2.0;
    
    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;
    
    b2Body* groundLineBody = world->CreateBody(&myGroundDef);
    
    b2EdgeShape edgeShape;
    edgeShape.Set( startpoint, endpoint ); // length -> coordinate vector from to vector
    
    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);
    
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites)
        return EXIT_FAILURE;
    
    SDL_Texture* texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    
    // cartesian origin box
    float x_box = -2.5f;
    float y_box = -2.5f;
    
    // size of box
    float w_box = 0.3;
    float h_box = 0.3;
    
    // angle of the box
    float angle_box = 45.0f; //45.0f;
    
    // Box
    SDL_Rect box;
    b2Body* Body;

    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.angle = angle_box; // flips the whole thing -> 180 grad drehung
    //boxBodyDef.angle = 0;
    boxBodyDef.position.Set(x_box, y_box);
    b2Vec2 vel;
    vel.Set(0, 0.2f);
    
    Body = world->CreateBody(&boxBodyDef);
    Body->SetLinearVelocity(vel);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((w_box / 2.0f) - dynamicBox.m_radius, (h_box / 2.0f) - dynamicBox.m_radius); // will be 0.5 x 0.5
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    Body->CreateFixture(&fixtureDef);
    
    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
    
    // cartesian origin of _0019_PLATF.png 89 x 22
    float x_plat = -3.6f; // to edge
    float y_plat = -0.14f;// to edge
    
    // size of the platform
    float w_plat = 89.0f / MET2PIX;
    float h_plat = 22.0f / MET2PIX;
    
    // define a Rect for this platform and its body def
    SDL_Rect platform;
    b2Body* Body_platform;
    
    b2BodyDef platformBoyDef;
    platformBoyDef.type = b2_staticBody;
    platformBoyDef.position.Set(x_plat, y_plat);
    
    Body_platform = world->CreateBody(&platformBoyDef);
    
    b2PolygonShape platformTile;    // subtracting radius fixes the incorrect little gap that can appear when working with really small resolutions
    platformTile.SetAsBox((w_plat / 2.0f) - platformTile.m_radius, (h_plat / 2.0f) - platformTile.m_radius); // subtracting the radius kills the gap issue:
    b2FixtureDef fixturePlat;
    fixturePlat.shape = &platformTile;
    fixturePlat.density = 1.0f;
    fixturePlat.friction = 0.3f;
    fixturePlat.restitution = 0.5f;
    Body_platform->CreateFixture(&fixturePlat);
    
    // set the SDL_RECT rendering values
    platform.w = w_plat * MET2PIX;
    platform.h = h_plat * MET2PIX;
    platform.x = ((SCALED_WIDTH / 2.0f) + x_plat) * MET2PIX - platform.w / 2;
    platform.y = ((SCALED_HEIGHT / 2.0f) + y_plat) * MET2PIX - platform.h / 2;
    
    bool close_game = false;
    SDL_Event event;
    
    // The game Loop
    while(close_game != true)
    {
        b2Vec2 pos = Body->GetPosition(); // Body = Body from box
        float angle = Body->GetAngle();
        
        // RAD2Degree
        angle *= RAD2DEG;
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                close_game = true;
            
            else if(event.key.keysym.sym == SDLK_ESCAPE)
                close_game = true;
            
            else if (event.key.keysym.sym == SDLK_r)
            {
                Body->SetTransform(b2Vec2(x_box, y_box), angle_box);
                Body->SetLinearVelocity(vel);
            }
        }
        
        // question box, update x and y destination
        box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2;
        box.y = ((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2;
        
        // cout << "X of box:" << setprecision(20) << box.x << endl;
        // cout << "Y of box:" << setprecision(20) << box.y << endl;
        
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        
        // Draw ground platform
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
        
        SDL_RenderCopyEx(renderer, texture_box, NULL, &box, Body->GetAngle() * RAD2DEG, NULL, SDL_FLIP_NONE);
        
        // Draw ziegl_3
        
        // Draw box angle 45
        //Body->SetAngularVelocity(10.0f);
        //Body->SetFixedRotation(true);
        //SDL_RenderDrawRect(renderer, &box);
        //SDL_RenderFillRect(renderer, &box);
        
        SDL_SetRenderDrawColor(renderer, 32, 70, 49, 0);
        SDL_RenderPresent(renderer);
        
        world->Step(1.0f / 60.0f, 6.0f, 2.0f); // update
        
    }
    
    // box2D delete whole world and free memory
    delete world;
    
    SDL_DestroyTexture(texture_box);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
