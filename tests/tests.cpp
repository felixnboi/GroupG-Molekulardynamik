#include <gtest/gtest.h>

#include "../src/forces/Lennard_Jones_Force.h"
#include "../src/inputFileManager.h"
#include "../src/particlegen/ParticleGenerator.h"
#include "../src/io/input/FileReader.h"
#include "../src/particlecontainers/ParticleContainerOld.h"
#include "../src/particlecontainers/ParticleContainer.h"
#include "../src/particlecontainers/ParticleContainerLinkedCell.h"
#include "../src/data/Cuboid.h"
#include "../src/data/Disc.h"
#include "../src/data/ThermostatData.h"
#include "../src/thermostat/Thermostat.h"
#include "../src/io/output/CheckpointWriter.h"


const double EPSILON = 1e-5;  // Tolerance
// A helper function
bool areDoublesEqual(double a, double b, double epsilon = EPSILON) {
    return std::fabs(a - b) < epsilon;
}

// Test case for adding particles and getting particles
TEST(ParticleContainerOld, AddAndGetParticles) {
    ParticleContainer* pc = new ParticleContainerOld();
    
    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle> ((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle> ((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);
    
    // Get particles from the container
    const std::vector<std::shared_ptr<Particle>>& particles = pc->getParticles();
    
    // Check if the number of particles matches
    EXPECT_EQ(particles.size(), 3);
    
    // Check if the particles in the container match the added particles
    //EXPECT_TRUE(compareParticles(particles[0], p1));
    EXPECT_TRUE(particles[0]==p1);
    EXPECT_TRUE(particles[1]==p2);
    EXPECT_TRUE(particles[2]==p3);
    delete pc;
}

// Test case for iterators
TEST(ParticleContainerOld, IteratorBeginEnd) {
    ParticleContainer* pc = new ParticleContainerOld();
    
    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle> ((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    
    // Test begin iterator
    ParticleIterator beginIter = pc->begin();
    EXPECT_TRUE(*beginIter==p1);
    
    // Test end iterator
    ParticleIterator endIter = pc->end();
    EXPECT_NE(beginIter, endIter); 
    delete pc;
}

// Test case for getParticlePairs
TEST(ParticleContainerOld, GetParticlePairs) {
    ParticleContainer* pc = new ParticleContainerOld();
    
    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);
    
    // Get particle pairs from the container
    std::vector<std::array<std::shared_ptr<Particle>, 2>> particlePairs = pc->getParticlePairs();
    
    // Check if the number of pairs matches
    EXPECT_EQ(particlePairs.size(), 3);
    
    // Check if the pairs are correct
    EXPECT_TRUE((particlePairs[0][0] == p1 && particlePairs[0][1] == p2) ||
                (particlePairs[0][0] == p2 && particlePairs[0][1] == p1));
    EXPECT_TRUE((particlePairs[1][0] == p1 && particlePairs[1][1] == p3) ||
                (particlePairs[1][0] == p3 && particlePairs[1][1] == p1));
    EXPECT_TRUE((particlePairs[2][0] == p2 && particlePairs[2][1] == p3) ||
                (particlePairs[2][0] == p3 && particlePairs[2][1] == p2));
    
    delete pc;
}


// Test case for addParticle and getParticles methods
TEST(ParticleContainerLinkedCell, AddAndGetParticles) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainer* pc = new ParticleContainerLinkedCell(sizeX, sizeY, sizeZ, radius);
    
    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);
    
    // Get particles from the container
    const std::vector<std::shared_ptr<Particle>>& particles = pc->getParticles();
    
    // Check if the number of particles matches
    EXPECT_EQ(particles.size(), 3);
    
    // Check if the particles in the container match the added particles
    EXPECT_TRUE(particles[0] == p1);
    EXPECT_TRUE(particles[1] == p2);
    EXPECT_TRUE(particles[2] == p3);
    
    delete pc;
}

// Test case for iterators
TEST(ParticleContainerLinkedCell, IteratorBeginEnd) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainer* pc = new ParticleContainerLinkedCell(sizeX, sizeY, sizeZ, radius);
    
    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    
    // Test begin iterator
    ParticleIterator beginIter = pc->begin();
    EXPECT_TRUE(*beginIter == p1);
    
    // Test end iterator
    ParticleIterator endIter = pc->end();
    EXPECT_NE(beginIter, endIter); 
    
    delete pc;
}

// Test case for getParticlePairs
TEST(ParticleContainerLinkedCell, GetParticlePairs) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainer* pc = new ParticleContainerLinkedCell(sizeX, sizeY, sizeZ, radius);

    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.1}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.05, 3.05}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 1, (std::array<double, 3>){0,0,0});

    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);

    // Get particle pairs from the container
    std::vector<std::array<std::shared_ptr<Particle>, 2>> particlePairs = pc->getParticlePairs();

    // Check if the number of pairs matches
    EXPECT_EQ(particlePairs.size(), 3); 

    // Verify specific pairs
    bool foundPair12 = false, foundPair13 = false, foundPair23 = false;
    for (const auto& pair : particlePairs) {
        if ((pair[0] == p1 && pair[1] == p2) || (pair[0] == p2 && pair[1] == p1)) {
            foundPair12 = true;
        } else if ((pair[0] == p1 && pair[1] == p3) || (pair[0] == p3 && pair[1] == p1)) {
            foundPair13 = true;
        } else if ((pair[0] == p2 && pair[1] == p3) || (pair[0] == p3 && pair[1] == p2)) {
            foundPair23 = true;
        }
    }

    // Check if all expected pairs are found
    EXPECT_TRUE(foundPair12);
    EXPECT_TRUE(foundPair13);
    EXPECT_TRUE(foundPair23);

    delete pc;
}

// Test case for getSize method
TEST(ParticleContainerLinkedCell, GetSize) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainerLinkedCell pc(sizeX, sizeY, sizeZ, radius);
    
    // Get the size of the container
    std::array<double, 3> size = pc.getSize();
    
    // Check if the size matches the expected values
    EXPECT_EQ(size[0], sizeX);
    EXPECT_EQ(size[1], sizeY);
    EXPECT_EQ(size[2], sizeZ);
}

// Test case for getCellSize method
TEST(ParticleContainerLinkedCell, GetCellSize) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainerLinkedCell pc(sizeX, sizeY, sizeZ, radius);
    
    // Get the cell size of the container
    std::array<double, 3> cellSize = pc.getCellSize();
    
    // Check if the cell size matches the expected values
    EXPECT_EQ(cellSize[0], sizeX / ceil(sizeX / radius));
    EXPECT_EQ(cellSize[1], sizeY / ceil(sizeY / radius));
    EXPECT_EQ(cellSize[2], sizeZ / ceil(sizeZ / radius));
}

//Test case for updateLoctions method
TEST(ParticleContainerLinkedCell, UpdateLocations) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainerLinkedCell pc(sizeX, sizeY, sizeZ, radius);

    // Create a particle that will move outside the boundary
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){10.0, 0.0, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add the particle to the container
    pc.addParticle(p1);
    p1->setX({11,2,3});
    // Set the outflow flags
    std::array<bool, 6> outflowFlag = {true, true, true, true, true, true};
    std::array<bool, 3> noPeridicFlags = {false, false, false};

    // Update the locations
    pc.updateLoctions(outflowFlag, {false, false, false});
    
    // Check if the particle has been moved to the halo region
    std::vector<std::shared_ptr<Particle>> halo = pc.getHalo();
    EXPECT_EQ(halo.size(), 1);
    if(halo.size()>=1) {
        EXPECT_TRUE(halo[0] == p1);
    }
}

// Test case for getBoundary method
TEST(ParticleContainerLinkedCell, GetBoundary) {
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 1.0;
    ParticleContainerLinkedCell pc(sizeX, sizeY, sizeZ, radius);

    // Create particles at the boundaries
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){0.0, 0.0, 0.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){9.0, 9.0, 9.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.0, 1, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){5.0, 5.0, 5.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.5, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add the particles to the container
    pc.addParticle(p1);
    pc.addParticle(p2);
    pc.addParticle(p3);
    
    // Get the boundary particles from the container
    std::vector<std::shared_ptr<Particle>> boundary = pc.getBoundary();
    
    // Check if the boundary particles match the expected values
    EXPECT_EQ(boundary.size(), 2);
    if(boundary.size()>=2) {
        EXPECT_TRUE(boundary[0] == p1 || boundary[1] == p1);
        EXPECT_TRUE(boundary[0] == p2 || boundary[1] == p2);
    }
}

TEST(ParticleGenerator, GenerateCuboid){
    // Reset the input file before starting the test
    inputFileManager::resetFile("../input/generated-input.txt");
    // Generate particles in a cuboid and write to the file
    ParticleGenerator::generateCuboid(Cuboid({2,2,2},{2,2,2},{2,2,2},2,2,0.1, 5, 1, 2, 0), "../input/generated-input.txt");
     // Open the generated input file for reading
    std::ifstream input_file("../input/generated-input.txt");

    if (!input_file.is_open()) {
        std::cerr << "Something went wrong, generated input file could not be opened\n";
        exit(-1);
    }

    std::string currentLine;
    // Verify the header lines in the file
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# Inputfile where all used particles will be stored.");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# there have to follow the exact quantity of spaces so that the number of chars in the line");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# adds up to 32 (not counting the \"\\n\" at the end)");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "8                               ");
    // Maxwell-Boltzmann numbers for velocity adjustments
    std::array<std::array<double, 3>, 8> maxwellBoltzmannNumbers = {{
        {-0.171411, 0.0178057, 0}, {0.00571789, -0.14098, 0},
        {0.0756284, -0.0582274, 0}, {-0.160245, -0.0304332, 0},
        {0.101672, -0.05844, 0}, {-0.0104494, -0.108908, 0},
        {-0.0279367, -0.00286395, 0}, {-0.0690031, 0.119264, 0}
    }};

    double baseX = 2.0;
    double baseY = 2.0;
    double baseZ = 2.0;
    double distance = 2.0;
    // Verifying each particle's data in the file
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                std::getline(input_file, currentLine);
                std::istringstream lineStream(currentLine);

                double x, y, z, vx, vy, vz, mass;
                lineStream >> x >> y >> z >> vx >> vy >> vz >> mass;
                // Calculate expected position and velocity based on input parameters
                double expectedX = baseX + i * distance;
                double expectedY = baseY + j * distance;
                double expectedZ = baseZ + k * distance;
                size_t idx = i * 4 + j * 2 + k;
                double expectedVx = 2 + maxwellBoltzmannNumbers[idx][0];
                double expectedVy = 2 + maxwellBoltzmannNumbers[idx][1];
                double expectedVz = 2 + maxwellBoltzmannNumbers[idx][2];

                // Verify that the generated values match the expected values with the specified tolerance
                EXPECT_TRUE(areDoublesEqual(x, expectedX));
                EXPECT_TRUE(areDoublesEqual(y, expectedY));
                EXPECT_TRUE(areDoublesEqual(z, expectedZ));
                EXPECT_TRUE(areDoublesEqual(vx, expectedVx));
                EXPECT_TRUE(areDoublesEqual(vy, expectedVy));
                EXPECT_TRUE(areDoublesEqual(vz, expectedVz));
                EXPECT_EQ(mass, 2);
            }
        }
    }
    // Close the input file after verification
    input_file.close();
    // Reset the input file again to its initial state
    inputFileManager::resetFile("../input/generated-input.txt");
}

TEST(ParticleGenerator, GenerateDisc) {
    // Reset the input file before starting the test
    inputFileManager::resetFile("../input/generated-input.txt");

    // Generate particles in a disc and write to the file
    ParticleGenerator::generateDisc(Disc({5.0, 5.0, 5.0},{1.0, 1.0, 1.0}, 5, 1.0, 2.0, 5, 1, 2, 0), "../input/generated-input.txt");

    // Open the generated input file for reading
    std::ifstream input_file("../input/generated-input.txt");

    if (!input_file.is_open()) {
        std::cerr << "Something went wrong, generated input file could not be opened\n";
        exit(-1);
    }

    std::string currentLine;
    // Verify the header lines in the file
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# Inputfile where all used particles will be stored.");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# there have to follow the exact quantity of spaces so that the number of chars in the line");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "# adds up to 32 (not counting the \"\\n\" at the end)");
    std::getline(input_file, currentLine);
    EXPECT_EQ(currentLine, "81                              "); 

    double baseX = 5.0;
    double baseY = 5.0;
    double baseZ = 5.0;
    double distance = 1.0;

    // Verifying each particle's data in the file
    for (size_t i = -5; i < 5; ++i) {
        for (size_t j = -5; j < 5; ++j) {
            std::getline(input_file, currentLine);
            std::istringstream lineStream(currentLine);

            double x, y, z, vx, vy, vz, mass;
            lineStream >> x >> y >> z >> vx >> vy >> vz >> mass;

            // Calculate expected position and velocity based on input parameters
            double expectedX = baseX + i * distance;
            double expectedY = baseY + j * distance;
            double expectedZ = baseZ;
            double expectedVx = 1.0;
            double expectedVy = 1.0;
            double expectedVz = 1.0;

            // Verify that the generated values match the expected values with the specified tolerance
            EXPECT_TRUE(areDoublesEqual(x, expectedX));
            EXPECT_TRUE(areDoublesEqual(y, expectedY));
            EXPECT_TRUE(areDoublesEqual(z, expectedZ));
            EXPECT_TRUE(areDoublesEqual(vx, expectedVx));
            EXPECT_TRUE(areDoublesEqual(vy, expectedVy));
            EXPECT_TRUE(areDoublesEqual(vz, expectedVz));
            EXPECT_EQ(mass, 2.0); 
        }
    }
    // Close the input file after verification
    input_file.close();
    // Reset the input file again to its initial state
    inputFileManager::resetFile("../input/generated-input.txt");
}

 TEST(inputFileManager, ResetInputFile){;
    std::fstream input_file;
    const char* test_filename = "../input/generated-input.txt";
    input_file.open(test_filename, std::ios::out | std::ios::trunc);
    if (!input_file.is_open()) {
        std::cerr << "something went wrong, generated input file could not be opened\n";
        FAIL();
    }
    input_file << "tmp";
    input_file.close();
    inputFileManager::resetFile(test_filename);

    input_file.open(test_filename, std::ios::in);
    if (!input_file.is_open()) {
        std::cerr << "something went wrong, generated input file could not be opened\n";
        FAIL();  // Fail the test if the file cannot be opened
    }

    std::stringstream buffer;
    buffer << input_file.rdbuf();
    std::string content = buffer.str();
    input_file.close();

    std::string expected_content = "# Inputfile where all used particles will be stored.\n"
                                   "# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles\n"
                                   "# there have to follow the exact quantity of spaces so that the number of chars in the line\n"
                                   "# adds up to 32 (not counting the \"\\n\" at the end)\n"
                                   "0                               \n";

    EXPECT_EQ(content, expected_content);
}

TEST(inputFileManager, MergeFile){
    const char* generated_filename = "../input/generated-input.txt";
    const char* merge_filename = "../input/eingabe-sonne.txt";

    std::ofstream input_file_1(generated_filename, std::ios::trunc);
    if (!input_file_1.is_open()) {
        std::cerr << "something went wrong, generated input file could not be opened\n";
        FAIL();
    }
    input_file_1 << "#test\n#test\n1\n0 0 0 0 0 0 0\n";
    input_file_1.close();

    inputFileManager::mergeFile(generated_filename, merge_filename);

    std::ifstream input_file_2(generated_filename);
    if (!input_file_2.is_open()) {
        std::cerr << "something went wrong, generated input file could not be opened\n";
        FAIL();  // Fail the test if the file cannot be opened
    }
    // Read the content of the file
    std::stringstream buffer;
    buffer << input_file_2.rdbuf();
    input_file_2.close();
    std::string content = buffer.str();
    std::string expected_content = "#test\n#test\n5\n0 0 0 0 0 0 0\n"
                                   "0.0 0.0 0.0      0.0 0.0 0.0     1.0\n"
                                   "0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6\n"
                                   "0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4\n"
                                   "34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14\n";

    // Compare the actual content with the expected content
    EXPECT_EQ(content, expected_content);
    inputFileManager::resetFile(generated_filename);
}

TEST(Lennard_Jones_Force, LennardJonesForce){
    ParticleContainer* particles = new ParticleContainerOld();
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){0,0,0}, 
    (std::array<double, 3>){0,0,0},1, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle> ((std::array<double, 3>){1,0,0}, 
    (std::array<double, 3>){0,0,0},1, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle> ((std::array<double, 3>){0,1,0}, 
    (std::array<double, 3>){0,0,0},1, 0, 5, 1, (std::array<double, 3>){0,0,0});
    particles->addParticle(p1);
    particles->addParticle(p2);
    particles->addParticle(p3);

    Lennard_Jones_Force LJForce {{false, false, false, false, false, false},{false, false, false}};
    LJForce.calculateF(*particles, false, 0);

    const auto& updatedParticles = particles->getParticles();

    // Verify forces on each particle
    const auto& p1_updated = updatedParticles[0];
    const auto& p2_updated = updatedParticles[1];
    const auto& p3_updated = updatedParticles[2];

    // Expected forces (to be calculated correctly)
    std::array<double, 3> expectedF1 = {-120, -120, 0};
    std::array<double, 3> expectedF2 = {114.375, 5.625, 0};
    std::array<double, 3> expectedF3 = {5.625, 114.375, 0};

    spdlog::info("p1 force: ({}, {}, {})", p1_updated->getF()[0], p1_updated->getF()[1], p1_updated->getF()[2]);
    spdlog::info("p2 force: ({}, {}, {})", p2_updated->getF()[0], p2_updated->getF()[1], p2_updated->getF()[2]);
    spdlog::info("p3 force: ({}, {}, {})", p3_updated->getF()[0], p3_updated->getF()[1], p3_updated->getF()[2]);

    // Check if the calculated forces are as expected
    EXPECT_NEAR(p1_updated->getF()[0], expectedF1[0], EPSILON);
    EXPECT_NEAR(p1_updated->getF()[1], expectedF1[1], EPSILON);
    EXPECT_NEAR(p1_updated->getF()[2], expectedF1[2], EPSILON);

    EXPECT_NEAR(p2_updated->getF()[0], expectedF2[0], EPSILON);
    EXPECT_NEAR(p2_updated->getF()[1], expectedF2[1], EPSILON);
    EXPECT_NEAR(p2_updated->getF()[2], expectedF2[2], EPSILON);

    EXPECT_NEAR(p3_updated->getF()[0], expectedF3[0], EPSILON);
    EXPECT_NEAR(p3_updated->getF()[1], expectedF3[1], EPSILON);
    EXPECT_NEAR(p3_updated->getF()[2], expectedF3[2], EPSILON);

    delete particles;
}

TEST(FileReader, readFile){
    ParticleContainer* particles = new ParticleContainerOld();
    FileReader fileReader;
    fileReader.readFile(*particles, "../input/eingabe-sonne.txt", {0,0,0});

    std::array<std::array<double, 7>, 4> pValues = {{
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, -1.0, 0.0, 0.0, 3.0e-6},
        {0.0, 5.36, 0.0, -0.425, 0.0, 0.0, 9.55e-4},
        {34.75, 0.0, 0.0, 0.0, 0.0296, 0.0, 1.0e-14}
    }};
    
    ASSERT_EQ(particles->getParticles().size(), 4); // Ensure we have the expected number of particles

    int i = 0;
    for(auto particle = particles->begin(); particle != particles->end(); ++particle, ++i){
        EXPECT_EQ((*particle)->getX()[0], pValues[i][0]);
        EXPECT_EQ((*particle)->getX()[1], pValues[i][1]);
        EXPECT_EQ((*particle)->getX()[2], pValues[i][2]);
        EXPECT_EQ((*particle)->getV()[0], pValues[i][3]);
        EXPECT_EQ((*particle)->getV()[1], pValues[i][4]);
        EXPECT_EQ((*particle)->getV()[2], pValues[i][5]);
        EXPECT_EQ((*particle)->getM(), pValues[i][6]);
    }

    delete particles;
}

TEST(Lennard_Jones_Force, LennardJonesReflection){
    ParticleContainerLinkedCell particles(10.0,10.0,10.0,2.5);
        
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){9.75,7.0,5.0}, 
    (std::array<double, 3>){0,0,0},1, 0, 5, 1, (std::array<double, 3>){0,0,0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle> ((std::array<double, 3>){9.0,3.0,5.0}, 
    (std::array<double, 3>){0,0,0},1, 0, 5, 1, (std::array<double, 3>){0,0,0});
    particles.addParticle(p1);
    particles.addParticle(p2);
    Lennard_Jones_Force force{{false,true,false,false,false,false}, {false, false, false}};
    force.calculateF(particles, true, 0);
    EXPECT_EQ(p1->getF()[0], -1950720); //test against manually calculated value
    EXPECT_EQ(p2->getF()[0], 0);
}

TEST(ParticleContainerLinkedCell, MirrorBoundary){
    ParticleContainerLinkedCell particles(10.0,10.0,10.0,2.5);
        
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){5.0,5.0,5.0}, 
    (std::array<double, 3>){0,0,0},1, 1, 5, 1, (std::array<double, 3>){0,0,0});
    particles.addParticle(p1);
    p1->setX({11.0,3.0,3.0});
    p1->setV({1.0,1.0,1.0});
    particles.updateLoctions({true,false,true,true,true,true}, {false, false, false});
    std::array<double, 3UL> positionAfterMirroring = {9.0,3.0,3.0};
    std::array<double, 3UL> velocityAfterMirroring = {-1.0,1.0,1.0};
    EXPECT_EQ(p1->getX(), positionAfterMirroring); 
    EXPECT_EQ(p1->getV(), velocityAfterMirroring); 
    EXPECT_EQ(particles.getHalo().size(),0);
}

TEST(ParticleContainerLinkedCell, OutflowBoundary){
    ParticleContainerLinkedCell particles(10.0,10.0,10.0,2.5);
        
    std::shared_ptr<Particle> p1 = std::make_shared<Particle> ((std::array<double, 3>){5.0,5.0,5.0}, 
    (std::array<double, 3>){0,0,0},1, 1, 5, 1, (std::array<double, 3>){0,0,0});
    particles.addParticle(p1);
    p1->setX({11.0,3.0,3.0});
    particles.updateLoctions({false,true,false,false,false,false}, {false, false, false});
    std::array<double, 3UL> positionAfterMirroring = {11.0,3.0,3.0};
    EXPECT_EQ(p1->getX(), positionAfterMirroring); 
    EXPECT_EQ(particles.getHalo().size(),1);
}

// Constants for the tests
const double INITIAL_TEMP = 1.0;
const double TARGET_TEMP = 2.0;
const double TARGET_TEMP_2 = 0.05;
const double MAX_DELTA_TEMP = 0.5;
const size_t N_THERMOSTAT = 100;
const size_t DIMENSIONS = 3;

// Helper function to create a particle container with some particles
std::unique_ptr<ParticleContainer> createParticleContainer(double sizeX, double sizeY, double sizeZ, double radius) {
    auto pc = std::make_unique<ParticleContainerLinkedCell>(sizeX, sizeY, sizeZ, radius);

    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);

    return pc;
}

// Test case for heating
TEST(Thermostat, Heating) {
    auto pc = createParticleContainer(10.0, 10.0, 10.0, 1.0);
    ThermostatData thermostat_data(true, N_THERMOSTAT, DIMENSIONS, true, TARGET_TEMP, MAX_DELTA_TEMP, true, INITIAL_TEMP);
    Thermostat thermostat(thermostat_data);

    double initialTemp = thermostat.getCurrentTemperature(pc);
    spdlog::info("initial Temp:{}", initialTemp);

    thermostat.scaleWithBeta(pc);
    double newTemp = thermostat.getCurrentTemperature(pc);
    spdlog::info("new Temp:{}", newTemp);

    EXPECT_GT(newTemp, initialTemp);
    EXPECT_LE(newTemp, initialTemp + MAX_DELTA_TEMP + EPSILON);
}

// Test case for cooling
TEST(Thermostat, Cooling) {
    auto pc = createParticleContainer(10.0, 10.0, 10.0, 1.0);
    ThermostatData thermostat_data(true, N_THERMOSTAT, DIMENSIONS, true, TARGET_TEMP_2, MAX_DELTA_TEMP, true, INITIAL_TEMP);
    Thermostat thermostat(thermostat_data);

    double initialTemp = thermostat.getCurrentTemperature(pc);
    thermostat.scaleWithBeta(pc);

    double newTemp = thermostat.getCurrentTemperature(pc);

    EXPECT_LT(newTemp, initialTemp);
    EXPECT_GE(newTemp, initialTemp - MAX_DELTA_TEMP);
}

// Test case for holding a temperature
TEST(Thermostat, HoldTemperature) {
    auto pc = createParticleContainer(10.0, 10.0, 10.0, 1.0);

    ThermostatData thermostat_data(true, N_THERMOSTAT, DIMENSIONS, true, 0.17500000000000002, MAX_DELTA_TEMP, true, INITIAL_TEMP);
    Thermostat thermostat(thermostat_data);

    double initialTemp = thermostat.getCurrentTemperature(pc);
    spdlog::info("initial Temp:{}", initialTemp);
    thermostat.scaleWithBeta(pc);

    double newTemp = thermostat.getCurrentTemperature(pc);

    EXPECT_NEAR(newTemp, initialTemp, EPSILON);
}

// Test case for initializing system temperature
TEST(Thermostat, InitSystemTemperature) {
    std::unique_ptr<ParticleContainer> pc1 = std::make_unique<ParticleContainerLinkedCell>(10.0, 10.0, 10.0, 1.0);
    std::unique_ptr<ParticleContainer> pc2= std::make_unique<ParticleContainerLinkedCell>(10.0, 10.0, 10.0, 1.0);

    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.0, 1, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.0, 0.0, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});

    std::shared_ptr<Particle> p4 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){1.39855, -2.31087, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p5 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){-0.835396, -0.327661, 0.0}, 1.0, 1, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p6 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.329751, 0.0308074, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    
    // Add particles to the container
    pc1->addParticle(p1);
    pc1->addParticle(p2);
    pc1->addParticle(p3);

    pc2->addParticle(p4);
    pc2->addParticle(p5);
    pc2->addParticle(p6);

    ThermostatData thermostat_data(true, N_THERMOSTAT, 2, true, TARGET_TEMP, MAX_DELTA_TEMP, true, INITIAL_TEMP);
    Thermostat thermostat(thermostat_data);

    spdlog::info("initial Temp:{}", thermostat.getCurrentTemperature(pc1));

    double newInitialTemp = 3.0;
    thermostat.initSystemTemperature(newInitialTemp, pc1);

    double newTemp = thermostat.getCurrentTemperature(pc1);
    double actualTemp = thermostat.getCurrentTemperature(pc2);

    EXPECT_NEAR(newTemp, actualTemp, EPSILON);
}

//Test for the gravity on the y axis
TEST(Lennard_Jones_Force, gravity){
    auto pc = std::make_unique<ParticleContainerLinkedCell>(10, 10, 10, 1);
    std::shared_ptr<Particle> p = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    pc->addParticle(p);
    Lennard_Jones_Force LJForce {{false, false, false, false, false, false},{false, false, false}};
    LJForce.calculateF(*pc, false, 10);
    EXPECT_EQ(p->getF()[0], 0);
    EXPECT_EQ(p->getF()[1], 10);
    EXPECT_EQ(p->getF()[2], 0);
}

//Tests if particles correctly attract each other through a periodic boundery
TEST(Lennard_Jones_Force, peridicBoundary){
    auto pc = std::make_unique<ParticleContainerLinkedCell>(10, 10, 10, 2);
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){0.5, 1, 1}, 
    (std::array<double, 3>){0, 0, 0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){9.5, 1, 1}, 
    (std::array<double, 3>){0, 0, 0}, 1.0, 0, 5, 1, (std::array<double, 3>){0, 0, 0});

    pc->addParticle(p1);
    pc->addParticle(p2);
    Lennard_Jones_Force LJForce {{false, false, false, false, false, false},{true, false, false}};
    LJForce.calculateF(*pc, true, 0);
    EXPECT_EQ(p1->getF()[0], 120);
    EXPECT_EQ(p1->getF()[1], 0);
    EXPECT_EQ(p1->getF()[2], 0);
    EXPECT_EQ(p2->getF()[0], -120);
    EXPECT_EQ(p2->getF()[1], 0);
    EXPECT_EQ(p2->getF()[2], 0);
}

//Tests that perticles that leave a peridic boundery are placed at the correct point on the other side of the domain
TEST(ParticleContainerLinkedCell, peridicBoundary){
    double sizeX = 10.0, sizeY = 10.0, sizeZ = 10.0, radius = 2.0;
    ParticleContainerLinkedCell pc(sizeX, sizeY, sizeZ, radius);

    // Create a particle that will move outside the boundary
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){5, 1, 1}, 
    (std::array<double, 3>){10.0, 0.0, 0.0}, 1.0, 0, 5, 1, (std::array<double, 3>){0,0,0});
    
    // Add the particle to the container
    pc.addParticle(p1);
    p1->setX({11,1,1});
    // Set the correct flags
    std::array<bool, 6> outflowFlag = {false, false, false, false, false, false};
    std::array<bool, 3> peridicFlags = {true, true, true};

    // Update the locations
    pc.updateLoctions(outflowFlag, peridicFlags);
    
    std::vector<std::shared_ptr<Particle>> halo = pc.getHalo();
    EXPECT_EQ(halo.size(), 0);
    std::array<double, 3> correctPlace = {1,1,1};
    EXPECT_EQ(p1->getX(), correctPlace);
}

TEST(CheckpointWriter, writeCheckpoint){
    ParticleContainer* pc = new ParticleContainerLinkedCell(10, 10, 10, 2);

    // Create some particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>((std::array<double, 3>){1.0, 2.0, 3.0}, 
    (std::array<double, 3>){0.1, 0.2, 0.3}, 1.0, 0, 1, 2, (std::array<double, 3>){0, 0, 0});
    p1->setF({1,2,3});
    p1->setOldF({11,22,33});
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>((std::array<double, 3>){2.0, 3.0, 4.0}, 
    (std::array<double, 3>){0.2, 0.3, 0.4}, 1.5, 1, 3, 4, (std::array<double, 3>){0, 0, 0});
    p2->setF({4,5,6});
    p2->setOldF({44,55,66});
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>((std::array<double, 3>){3.0, 4.0, 5.0}, 
    (std::array<double, 3>){0.3, 0.4, 0.5}, 2.0, 0, 5, 6, (std::array<double, 3>){0, 0, 0});
    p1->setF({7,8,9});
    p1->setOldF({77,88,99});
    
    // Add particles to the container
    pc->addParticle(p1);
    pc->addParticle(p2);
    pc->addParticle(p3);
    CheckpointWriter::writeCheckpoint(*pc, "../input/checkpointTest.txt");
    
    ParticleContainer* particles = new ParticleContainerOld();
    FileReader fileReader;
    fileReader.readFile(*particles, "../input/checkpointTest.txt", {0,0,0});
    auto p = particles->getParticles();
    assert(p[0]->getX()[0]==1&&p[0]->getX()[1]==2&&p[0]->getX()[2]==3);
    assert(p[1]->getX()[0]==2&&p[1]->getX()[1]==3&&p[1]->getX()[2]==4);
    assert(p[2]->getX()[0]==3&&p[2]->getX()[1]==4&&p[2]->getX()[2]==5);

    assert(p[0]->getV()[0]==0.1&&p[0]->getV()[1]==0.2&&p[0]->getV()[2]==0.3);
    assert(p[1]->getV()[0]==0.2&&p[1]->getV()[1]==0.3&&p[1]->getV()[2]==0.4);
    assert(p[2]->getV()[0]==0.3&&p[2]->getV()[1]==0.4&&p[2]->getV()[2]==0.5);

    assert(p[0]->getM()==1&&p[1]->getM()==1.5&&p[2]->getM()==2);

    assert(p[0]->getType()==0&&p[1]->getType()==1&&p[2]->getType()==0);

    assert(p[0]->getEpsilon()==1&&p[1]->getEpsilon()==3&&p[2]->getEpsilon()==5);

    assert(p[0]->getSigma()==2&&p[1]->getSigma()==4&&p[2]->getSigma()==6);

    assert(p[0]->getF()[0]==1&&p[0]->getF()[1]==2&&p[0]->getF()[2]==3);
    assert(p[1]->getF()[0]==4&&p[1]->getF()[1]==5&&p[1]->getF()[2]==6);
    assert(p[2]->getF()[0]==7&&p[2]->getF()[1]==8&&p[2]->getF()[2]==9);

    assert(p[0]->getOldF()[0]==11&&p[0]->getOldF()[1]==22&&p[0]->getOldF()[2]==33);
    assert(p[1]->getOldF()[0]==44&p[1]->getOldF()[1]==55&&p[1]->getOldF()[2]==66);
    assert(p[2]->getOldF()[0]==77&&p[2]->getOldF()[1]==88&&p[2]->getOldF()[2]==99);
    delete particles;
    delete pc;
}