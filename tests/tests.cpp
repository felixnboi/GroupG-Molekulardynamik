#include <gtest/gtest.h>

#include "../src/Particle.h"
#include "../src/ParticleContainer.h"

// Test case for adding particles and getting particles
TEST(ParticleContainerTest, AddAndGetParticles) {
    ParticleContainer pc;
    
    // Create some particles
    Particle p1({1.0, 2.0, 3.0}, {0.1, 0.2, 0.3}, 1.0, 0);
    Particle p2({2.0, 3.0, 4.0}, {0.2, 0.3, 0.4}, 1.5, 1);
    Particle p3({3.0, 4.0, 5.0}, {0.3, 0.4, 0.5}, 2.0, 0);
    
    // Add particles to the container
    pc.addParticle(p1);
    pc.addParticle(p2);
    pc.addParticle(p3);
    
    // Get particles from the container
    const std::vector<Particle>& particles = pc.getParticles();
    
    // Check if the number of particles matches
    EXPECT_EQ(particles.size(), 3);
    
    // Check if the particles in the container match the added particles
    //EXPECT_TRUE(compareParticles(particles[0], p1));
    EXPECT_TRUE(particles[0]==p1);
    EXPECT_TRUE(particles[1]==p2);
    EXPECT_TRUE(particles[2]==p3);
}

// Test case for iterators
TEST(ParticleContainerTest, IteratorBeginEnd) {
    ParticleContainer pc;
    
    // Create some particles
    Particle p1({1.0, 2.0, 3.0}, {0.1, 0.2, 0.3}, 1.0, 0);
    Particle p2({2.0, 3.0, 4.0}, {0.2, 0.3, 0.4}, 1.5, 1);
    
    // Add particles to the container
    pc.addParticle(p1);
    pc.addParticle(p2);
    
    // Test begin iterator
    ParticleIterator beginIter = pc.begin();
    EXPECT_TRUE(*beginIter==p1);
    
    // Test end iterator
    ParticleIterator endIter = pc.end();
    EXPECT_NE(beginIter, endIter); 
}