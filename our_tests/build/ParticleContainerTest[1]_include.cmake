if(EXISTS "/home/masha_lelyukh/BachelorPraktikum/GroupG-Molekulardynamik/our_tests/build/ParticleContainerTest[1]_tests.cmake")
  include("/home/masha_lelyukh/BachelorPraktikum/GroupG-Molekulardynamik/our_tests/build/ParticleContainerTest[1]_tests.cmake")
else()
  add_test(ParticleContainerTest_NOT_BUILT ParticleContainerTest_NOT_BUILT)
endif()
