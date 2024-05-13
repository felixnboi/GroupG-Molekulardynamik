#include <gtest/gtest.h>

#include "Particle.h"
#include "ParticleContainer.h"



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
    EXPECT_EQ(particles[0], p1);
    EXPECT_EQ(particles[1], p2);
    EXPECT_EQ(particles[2], p3);
}

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
    EXPECT_EQ(*beginIter, p1);
    
    // Test end iterator
    ParticleIterator endIter = pc.end();
    EXPECT_NE(beginIter, endIter); // Ensure that begin and end iterators are different
}




//TEST(HelloTest, BasicAssertions) {
  //EXPECT_EQ(7 * 6, 42);
  //EXPECT_TRUE(true);
  //EXPECT_TRUE(false);}
// rm test_suite[1]_include.cmake
//cmake GroupG-Molekulardynamik/
//make
//./teststotest 
