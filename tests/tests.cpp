#include <gtest/gtest.h>

#include "../src/Forces/Lennard_Jones_Force.h"
#include "../src/inputFileManager.h"
#include "../src/particlegen/ParticleGenerator.h"
#include "../src/FileReader.h"

const double EPSILON = 1e-5;  // Tolerance
// A helper function
bool areDoublesEqual(double a, double b, double epsilon = EPSILON) {
    return std::fabs(a - b) < epsilon;
}

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

TEST(ParticleGenerator, GenerateCuboid){
    // Reset the input file before starting the test
    inputFileManager::resetFile("../input/generated-input.txt");
    // Generate particles in a cuboid and write to the file
    ParticleGenerator::generateCuboid(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, "../input/generated-input.txt");
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
    //EXPECT_TRUE(currentLine, "8                               \n");
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
    ParticleContainer particles {};
    Particle p1 {{0,0,0},{0,0,0},1};
    Particle p2 {{1,0,0},{0,0,0},1};
    Particle p3 {{0,1,0},{0,0,0},1};
    particles.addParticle(p1);
    particles.addParticle(p2);
    particles.addParticle(p3);

    Lennard_Jones_Force lJForce {};
    lJForce.calculateF(particles);

    const auto& updatedParticles = particles.getParticles();

    // Verify forces on each particle
    const auto& p1_updated = updatedParticles[0];
    const auto& p2_updated = updatedParticles[1];
    const auto& p3_updated = updatedParticles[2];

    // Expected forces (to be calculated correctly)
    std::array<double, 3> expectedF1 = {-120, -120, 0};
    std::array<double, 3> expectedF2 = {114.375, 5.625, 0};
    std::array<double, 3> expectedF3 = {5.625, 114.375, 0};

    spdlog::info("p1 force: ({}, {}, {})", p1_updated.getF()[0], p1_updated.getF()[1], p1_updated.getF()[2]);
    spdlog::info("p2 force: ({}, {}, {})", p2_updated.getF()[0], p2_updated.getF()[1], p2_updated.getF()[2]);
    spdlog::info("p3 force: ({}, {}, {})", p3_updated.getF()[0], p3_updated.getF()[1], p3_updated.getF()[2]);

    // Check if the calculated forces are as expected
    EXPECT_NEAR(p1_updated.getF()[0], expectedF1[0], EPSILON);
    EXPECT_NEAR(p1_updated.getF()[1], expectedF1[1], EPSILON);
    EXPECT_NEAR(p1_updated.getF()[2], expectedF1[2], EPSILON);

    EXPECT_NEAR(p2_updated.getF()[0], expectedF2[0], EPSILON);
    EXPECT_NEAR(p2_updated.getF()[1], expectedF2[1], EPSILON);
    EXPECT_NEAR(p2_updated.getF()[2], expectedF2[2], EPSILON);

    EXPECT_NEAR(p3_updated.getF()[0], expectedF3[0], EPSILON);
    EXPECT_NEAR(p3_updated.getF()[1], expectedF3[1], EPSILON);
    EXPECT_NEAR(p3_updated.getF()[2], expectedF3[2], EPSILON);
}

TEST(FileReader, readFile){
    ParticleContainer particles;
    FileReader fileReader;
    fileReader.readFile(particles, "../input/eingabe-sonne.txt");

    std::array<std::array<double, 7>, 4> pValues = {{
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, -1.0, 0.0, 0.0, 3.0e-6},
        {0.0, 5.36, 0.0, -0.425, 0.0, 0.0, 9.55e-4},
        {34.75, 0.0, 0.0, 0.0, 0.0296, 0.0, 1.0e-14}
    }};
    
    ASSERT_EQ(particles.getParticles().size(), 4); // Ensure we have the expected number of particles

    int i = 0;
    for(auto particle = particles.begin(); particle != particles.end(); ++particle, ++i){
        EXPECT_EQ(particle->getX()[0], pValues[i][0]);
        EXPECT_EQ(particle->getX()[1], pValues[i][1]);
        EXPECT_EQ(particle->getX()[2], pValues[i][2]);
        EXPECT_EQ(particle->getV()[0], pValues[i][3]);
        EXPECT_EQ(particle->getV()[1], pValues[i][4]);
        EXPECT_EQ(particle->getV()[2], pValues[i][5]);
        EXPECT_EQ(particle->getM(), pValues[i][6]);
    }
}

