# Steering Behaviors

<p align="center">
  <img src="assets/flocking.gif" width="500">
</p>

A demo showcasing autonomous steering behaviors, including seek/flee, arrival, pursuit/evade, wander, obstacle avoidance, and more.

## Overview

Inspired by "Steering Behaviors For Autonomous Characters" by Craig W. Reynolds, and pseudocode from "Programming Game AI by Example" by Mat Buckland.

## Features

### Seek / Flee
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Seek steers a vehicle toward a target position; Flee does the opposite, steering away from it.

### Arrival
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

A variation of Seek that decelerates smoothly as the vehicle approaches its target, rather than overshooting and correcting.

### Pursuit / Evade
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Pursuit predicts a moving target's future position and seeks toward it; Evade does the opposite, fleeing from a predicted future position.

### Wander
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Generates a smooth, randomized path so a vehicle can move around without a specific target.

### Obstacle Avoidance
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Steers a vehicle around obstacles detected in its path.

### Wall Avoidance
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Uses feeler rays to detect nearby walls and steers the vehicle away before it collides.

### Interpose
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Steers a vehicle to a position between two other agents, predicting their future positions.

### Hide
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Finds the nearest obstacle that blocks line of sight to a target and steers behind it for cover.

### Path Following
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Steers a vehicle along a series of waypoints, advancing to the next one as it gets close.

### Offset Pursuit
<img src="https://placehold.co/400x250?text=GIF+coming+soon" width="400">

Steers a vehicle to maintain a fixed position offset relative to a moving leader — useful for formations.

### Flocking
<img src="assets/flocking.gif" width="400">

Combines separation, alignment, and cohesion so a group of vehicles moves together as a coordinated flock.

## How it works

## Built with

- C++
- SFML