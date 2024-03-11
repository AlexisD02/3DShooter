# 3DShooter

## Description

This project presents a third-person shooter game prototype developed in Unreal Engine 5 using C++. The game features engaging mechanics, dynamic enemy AI, and immersive gameplay elements tailored to showcase the capabilities of Unreal Engine 5 for game development. Key features include:

- **Game Screens**: Title, Win, Lose, and Gameplay screens for a complete game experience.
- **Player Movement**: Implemented through a custom C++ Pawn class, enabling full navigational control within the game level.
- **Interactable Actors**: A C++ Actor class allowing players to engage with the game environment, including shooting and interacting with objects.
- **Collision Handling**: Comprehensive collision detection and response between game actors.
- **Enemy AI**: Basic enemy behaviors driving movement and interaction with the player.
- **Game Rules**: A C++ GameMode class enforcing game logic, including score tracking, difficulty scaling, and win/lose conditions.
- **Audio**: Rich in-game sound effects and music enhancing the gaming experience.
- **Third-Person Camera**: Utilizing Camera and SpringArm components for a third-person perspective, fully implemented in C++.

## Advanced Features

- **Blueprint Integration**: Extensive use of property and function specifiers for blueprint subclassing, enabling customizable gameplay elements.
- **HUD**: Displays vital player stats such as health and ammunition, initialized via a C++ PlayerController class.
- **Damage System**: Integration of the `UGameplayStatics::ApplyDamage` method for actor damage handling.
- **Projectile Mechanics**: Use of the `UProjectileComponent` for firing mechanics with damage application.
- **Timers and Triggers**: In-game timers and event-driven gameplay through trigger events.
- **Source Control**: Project versioning and collaboration via GitHub.

## Elite Mechanics

- **Destructible Environment**: Inclusion of destructible mesh actors that react to player actions.
- **Physics and Forces**: Application of physics-based forces for realistic interactions.
- **AI Navigation**: Pathfinding with Navmesh Bounds Volume and a sophisticated AI controller class.
- **Behavioral AI**: A Behavior Tree/Blackboard system managing enemy AI, enabling chase and attack strategies.
- **Custom AI Logic**: Custom-written Behavior Tree services and tasks for nuanced AI behaviors.
- **Mini-Map**: Implementation of a mini-map using SceneCaptureComponent and RenderTarget.
