#include <gtest/gtest.h>

#include "../src/Forces/Lenard_Jones_Force.h"
#include "../src/inputFileManager.h"
#include "../src/particlegen/ParticleGenerator.h"
#include "../src/FileReader.h"

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

// TEST(inputFileManager, ResetInputFile){;
//     std::fstream input_file;
//     input_file.open("../../input/generated-input.txt",std::ios::out|std::ios::in|std::ios::trunc);
//     if (!input_file.is_open()) {
//         std::cout << "something went wrong, generated input file could not be opend\n";
//         exit(-1);
//     }
//     input_file << "tmp";
//     input_file.close();
//     inputFileManager::resetFile("../../input/generated-input.txt");
//     input_file.open("../../input/generated-input.txt",std::ios::in);
//     input_file.seekg(0, std::ios::end);
//     int size = input_file.tellg();
//     input_file.seekg(0, std::ios::beg);
//     char *content = new char[size];
//     input_file.read(content, size);
//     EXPECT_TRUE(content=="# Inputfile where all used particles will be stored.\n# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles\n# there have to follow the exact quantity of spaces so that the number of chars in the line\n# adds up to 32 (not counting the \"\\n\" at the end)\n0                               \n");
//     input_file.close();
//     delete[] content;
// }

// TEST(inputFileManager, MergeFile){
//     std::fstream input_file;
//     input_file.open("../../input/generated-input.txt",std::ios::out|std::ios::trunc);
//     if (!input_file.is_open()) {
//         std::cout << "something went wrong, generated input file could not be opend\n";
//         exit(-1);
//     }
//     input_file << "#test\n#test\n1                               \n0 0 0 0 0 0 0";
//     inputFileManager::mergeFile("../../input/generated-input.txt","../../input/eingabe-sonne.txt");
//     input_file.seekg(0, std::ios::end);
//     int size = input_file.tellg();
//     input_file.seekg(0, std::ios::beg);
//     char *content = new char[size];
//     input_file.read(content, size);
//     EXPECT_TRUE(content=="#test\ntest\n1                               \n0 0 0 0 0 0 0\n0.0 0.0 0.0      0.0 0.0 0.0     1.0\n0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6 \n0.0 5.36 0.0    -0.425 0.0 0.0   9.55e-4\n34.75 0.0 0.0    0.0 0.0296 0.0  1.0e-14");
//     input_file.close();
//     delete[] content;
//     inputFileManager::resetFile("../../input/generated-input.txt");
// }

// TEST(ParticleGenerator, GenerateCuboit){
//     inputFileManager::resetFile("../../input/generated-input.txt");
//     ParticleGenerator::generateCuboid(2,2,2,2,2,2,2,2,2,2,2,"../../input/generated-input.txt");
//     std::fstream input_file;
//     input_file.open("../../input/generated-input.txt",std::ios::out|std::ios::trunc);
//     if (!input_file.is_open()) {
//         std::cout << "something went wrong, generated input file could not be opend\n";
//         exit(-1);
//     }
//     input_file.seekg(0, std::ios::beg);
//     std::string currentLine;
//     getline(input_file, currentLine);
//     EXPECT_TRUE(currentLine=="# Inputfile where all used particles will be stored.");
//     getline(input_file, currentLine);
//     EXPECT_TRUE(currentLine== "# Similar syntax to \"eingabe-sonne.txt\" with the exeption that after the number of particles");
//     getline(input_file, currentLine);
//     EXPECT_TRUE(currentLine== "# there have to follow the exact quantity of spaces so that the number of chars in the line");
//     getline(input_file, currentLine);
//     EXPECT_TRUE(currentLine== "# adds up to 32 (not counting the \"\\n\" at the end)");
//     getline(input_file, currentLine);
//     EXPECT_TRUE(currentLine== "8                               \n");

//     std::array<std::array <double, 3>, 8> maxwellBoltzmannNumbers = {{{-0.171411, 0.0178057, 0.00571789},{0.0756284, -0.0582274, -0.160245},{0.101672, -0.05844, -0.0104494},
//     {-0.0279367, -0.00286395, -0.0690031},{0.0807455, -0.133418, -0.0482316},{0.0190382, 0.00177866, 0.0398789},{-0.0403815, -0.124431, 0.075199},
//     {0.0134107, -0.238648, -0.0288894}}};
//     std::stringstream actualLine;
    
//     for(int i = 0; i< 8; i++) {
//         actualLine.str("2 2 2");
//         actualLine << "2 2 2 " << 2+maxwellBoltzmannNumbers[i][0] << " " << 2+maxwellBoltzmannNumbers[i][1] << " " << 2+maxwellBoltzmannNumbers[i][2] << " 2";
//         EXPECT_TRUE(currentLine==actualLine.str());
//     }
//     input_file.close();
//     inputFileManager::resetFile("../../input/generated-input.txt");
// }

// TEST(Lenard_Jones_Force, LenardJonesForce){
//     ParticleContainer particles {};
//     Particle p1 {{0,0,0},{0,0,0},1};
//     Particle p2 {{1,0,0},{0,0,0},1};
//     Particle p3 {{0,1,0},{0,0,0},1};
//     particles.addParticle(p1);
//     particles.addParticle(p2);
//     particles.addParticle(p3);
//     Lenard_Jones_Force lJForce {};
//     lJForce.calculateF(particles);
//     EXPECT_TRUE(p1.getF()[0] == -120 && p1.getF()[1] == -120 && p1.getF()[2] == 0);
//     EXPECT_TRUE(p2.getF()[0] == 120+5.625 && p1.getF()[1] == 0-5.625 && p1.getF()[2] == 0);
//     EXPECT_TRUE(p3.getF()[0] == 0-5.625 && p1.getF()[1] == 120+5.625 && p1.getF()[2] == 0);
// }

// TEST(FileReader, readFile){
//     ParticleContainer particles {};
//     FileReader fileReader{};
//     fileReader.readFile(particles, "../../input/eingabe-sonne.txt");
//     std::array<std::array <double, 7>, 4> pValues = {{{0,0,0,0,0,0,1},{0,1,0,-1,0,0,3.0e-6},{0,5.36,0.0,-0.425,0,0,9.55e-4},{34.75,0,0,0,0.0296,0,1.0e-14}}};
//     int i = 0;
//     for(ParticleIterator particle = particles.begin(); particle != particles.end();particle++){
//         EXPECT_TRUE((particle->getX()[0]==pValues[i][0] && particle->getX()[1]==pValues[i][1] && particle->getX()[2]==pValues[i][2] && particle->getV()[0]==pValues[i][3] && 
//             particle->getV()[1]==pValues[i][4] && particle->getV()[2]==pValues[i][5] && particle->getM() ==pValues[i][6]));
//     }
// }