#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "audio.h"

TEST_CASE("constructor with size", "[Constructor]"){
  audio<int8_t> a = audio<int8_t>(0);
  REQUIRE(a.get_buffer().size() == 0);
}

TEST_CASE("Constructor with vector", "[Constructor]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  REQUIRE(a.get_buffer()[0] == 1);
  REQUIRE(a.get_buffer()[1] == 2);
  REQUIRE(a.get_buffer()[2] == 3);
}

TEST_CASE("Constructor with vector and sample length", "[Constructor]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v, 6);
  REQUIRE(a.get_sample_length() == 6);
  REQUIRE(a.get_buffer()[0] == 1);
  REQUIRE(a.get_buffer()[1] == 2);
  REQUIRE(a.get_buffer()[2] == 3);
}

TEST_CASE("Copy Constructor", "[Copy Constructor]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  audio<int8_t> b(a);
  REQUIRE(b.get_buffer()[0] == 1);
  REQUIRE(b.get_buffer()[1] == 2);
  REQUIRE(b.get_buffer()[2] == 3);
}

TEST_CASE("Copy assignment operator", "[Copy Assignment Operator]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  audio<int8_t> b = a;
  REQUIRE(b.get_buffer()[0] == 1);
  REQUIRE(b.get_buffer()[1] == 2);
  REQUIRE(b.get_buffer()[2] == 3);
}

TEST_CASE("Move constructor", "[Move Constructor]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  audio<int8_t> b(std::move(a));
  SECTION("Variables in new audio vector"){
    REQUIRE(b.get_buffer()[0] == 1);
    REQUIRE(b.get_buffer()[1] == 2);
    REQUIRE(b.get_buffer()[2] == 3);
  }
  SECTION("Variables in old audio vector"){
    REQUIRE(a.get_buffer().size() == 0);
  }
}

TEST_CASE("Move assignment operator", "[Move Assignment Operator]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  audio<int8_t> b = std::move(a);
  SECTION("Variables in new audio vector"){
    REQUIRE(b.get_buffer()[0] == 1);
    REQUIRE(b.get_buffer()[1] == 2);
    REQUIRE(b.get_buffer()[2] == 3);
  }
  SECTION("Variables in old audio vector"){
    REQUIRE(a.get_buffer().size() == 0);
  }
}

TEST_CASE("[] operator", "operator[]"){
  std::vector<int8_t> v = {1, 2, 3};
  audio<int8_t> a = audio<int8_t>(v);
  REQUIRE(a.get_buffer()[0] == 1);
  REQUIRE(a.get_buffer()[1] == 2);
  REQUIRE(a.get_buffer()[2] == 3);
}

TEST_CASE("| operator", "[operator|]"){
  audio<int8_t> a, b, c;
  std::vector<int8_t> v = {1, 2, 3};
  b = a = audio<int8_t>(v);
  c = a | b;
  REQUIRE(c.get_buffer()[0] == 1);
  REQUIRE(c.get_buffer()[1] == 2);
  REQUIRE(c.get_buffer()[2] == 3);
  REQUIRE(c.get_buffer()[3] == 1);
  REQUIRE(c.get_buffer()[4] == 2);
  REQUIRE(c.get_buffer()[5] == 3);
}

TEST_CASE("* operator", "[operator*]"){
  audio<int8_t> a;
  std::pair<float, float> p = {0.5, 0.5};
  std::vector<int8_t> v = {1, 2, 3};
  a = audio<int8_t>(v);
  audio<int8_t> b = a * p;
  REQUIRE(b.get_buffer()[0] == 0);
  REQUIRE(b.get_buffer()[1] == 1);
  REQUIRE(b.get_buffer()[2] == 1);
}

TEST_CASE("+ operator", "[operator+]"){
  audio<int16_t> a, b, c;
  std::vector<int16_t> v = {1, 2, 20000};
  b = a = audio<int16_t>(v);
  c = a + b;
  REQUIRE(c.get_buffer()[0] == 2);
  REQUIRE(c.get_buffer()[1] == 4);
  REQUIRE(c.get_buffer()[2] == 32767);
}

TEST_CASE("^ operator", "[operator^]"){
  audio<int> a;
  std::pair<int, int> p = {1, 2};
  std::vector<int> v = {1, 2, 3, 4};
  a = audio<int>(v);
  audio<int> b = a ^ p;
  REQUIRE(b.get_buffer()[0] == 1);
  REQUIRE(b.get_buffer()[1] == 4);
}

TEST_CASE("Reverse", "[Reverse]"){
  audio<int8_t> a;
  std::vector<int8_t> v = {1, 2, 3};
  a = audio<int8_t>(v);
  SECTION("Before reverse"){
    REQUIRE(a.get_buffer()[0] == 1);
    REQUIRE(a.get_buffer()[1] == 2);
    REQUIRE(a.get_buffer()[2] == 3);
  }
  a.reverse();
  SECTION("After reverse"){
    REQUIRE(a.get_buffer()[0] == 3);
    REQUIRE(a.get_buffer()[1] == 2);
    REQUIRE(a.get_buffer()[2] == 1);
  }
}

  TEST_CASE("Ranged add", "[Ranged Add]"){
    audio<int8_t> a, b, c;
    std::pair<int, int> p1, p2;
    std::vector<int8_t> v = {1, 2, 3, 4};
    b = a = audio<int8_t>(v);
    p1 = p2 = {1, 3};
    c = a.ranged_add(p1, p2, b);
    REQUIRE(c.get_buffer()[0] == 4);
    REQUIRE(c.get_buffer()[1] == 6);
    REQUIRE(c.get_buffer()[2] == 8);
  }

  TEST_CASE("Calculate RMS", "[Calculate RMS]"){
    std::vector<int8_t> v = {1, 2, 3};
    audio<int8_t> a = audio<int8_t>(v);
    float s = a.calculate_rms();
    REQUIRE(s == (float)sqrt(14.0 / 3));
  }

  TEST_CASE("Normalize", "[Normalize]"){
    std::vector<int8_t> v = {1, 2, 3};
    audio<int8_t> a, b;
    a = audio<int8_t>(v);
    float rms = a.calculate_rms();
    b = a.normalize(rms, 7);
    REQUIRE(b.get_buffer()[0] == 3);
  }

  TEST_CASE("Fade in", "[Fade In]"){
    std::vector<int8_t> v = {10, 20, 30, 40, 50};
    audio<int8_t> a, b;
    a = audio<int8_t>(v, 2);
    b = a.fade_in(2);
    REQUIRE(b.get_buffer()[0] == 2);
    REQUIRE(b.get_buffer()[1] == 10);
    REQUIRE(b.get_buffer()[2] == 23);
    REQUIRE(b.get_buffer()[3] == 40);
    REQUIRE(b.get_buffer()[4] == 50);
  }

  TEST_CASE("Fade out", "[Fade Out]"){
    std::vector<int8_t> v = {10, 20, 30, 40, 50};
    audio<int8_t> a, b;
    a = audio<int8_t>(v, 2);
    b = a.fade_in(2);
    REQUIRE(b.get_buffer()[0] == 8);
    REQUIRE(b.get_buffer()[1] == 10);
    REQUIRE(b.get_buffer()[2] == 8);
    REQUIRE(b.get_buffer()[3] == 0);
    REQUIRE(b.get_buffer()[4] == 50);
  }

  TEST_CASE("DEFAULT CONSTRUCTOR", "[CONSTRUCTOR]"){
      audio<std::pair<int8_t, int8_t>> a = audio<std::pair<int8_t, int8_t>>(0);
      REQUIRE(a.get_buffer().size() == 0);
  }

  TEST_CASE("Stereo constructor with vector", "[Constructor]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a = audio<std::pair<int8_t, int8_t>>(v);
    REQUIRE(a.get_buffer()[0].first == 1);
    REQUIRE(a.get_buffer()[0].second == 2);
    REQUIRE(a.get_buffer()[1].first == 3);
    REQUIRE(a.get_buffer()[1].second == 4);
  }

  TEST_CASE("Stereo constructor with vector and sample_length", "[Constructor]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a = audio<std::pair<int8_t, int8_t>>(v, 2);
    REQUIRE(a.get_buffer()[0].first == 1);
    REQUIRE(a.get_buffer()[0].second == 2);
    REQUIRE(a.get_buffer()[1].first == 3);
    REQUIRE(a.get_buffer()[1].second == 4);
    REQUIRE(a.get_sample_length() == 2);
  }

  TEST_CASE("Stereo copy constructor with vector and sample_length", "[Constructor]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a;
    a = audio<std::pair<int8_t, int8_t>>(v);
    audio<std::pair<int8_t, int8_t>> b(a);
    REQUIRE(b.get_buffer()[0].first == 1);
    REQUIRE(b.get_buffer()[0].second == 2);
    REQUIRE(b.get_buffer()[1].first == 3);
    REQUIRE(b.get_buffer()[1].second == 4);
  }

  TEST_CASE("Stereo =operator", "[operator=]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a;
    a = audio<std::pair<int8_t, int8_t>>(v, 2);
    audio<std::pair<int8_t, int8_t>>  b = a;
    REQUIRE(b.get_buffer()[0].first == 1);
    REQUIRE(b.get_buffer()[0].second == 2);
    REQUIRE(b.get_buffer()[1].first == 3);
    REQUIRE(b.get_buffer()[1].second == 4);
  }

  TEST_CASE("Stereo move constructor with vector and sample_length", "[Constructor]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a;
    a = audio<std::pair<int8_t, int8_t>>(v, 2);
    audio<std::pair<int8_t, int8_t>> b = std::move(a);
    REQUIRE(b.get_buffer()[0].first == 1);
    REQUIRE(b.get_buffer()[0].second == 2);
    REQUIRE(b.get_buffer()[1].first == 3);
    REQUIRE(b.get_buffer()[1].second == 4);
    REQUIRE(a.get_buffer().size() == 0);
  }

  TEST_CASE("Stereo |operator", "[operator|]"){
    std::pair<int8_t, int8_t> p1 = {1, 2};
    std::vector<std::pair<int8_t, int8_t>> v = {p1};
    audio<std::pair<int8_t, int8_t>> a, b, c;
    b = a = audio<std::pair<int8_t, int8_t>>(v);
    c = a | b;
    REQUIRE(c.get_buffer()[0].first == 1);
    REQUIRE(c.get_buffer()[0].second == 2);
    REQUIRE(c.get_buffer()[1].first == 1);
    REQUIRE(c.get_buffer()[1].second == 2);
  }

  TEST_CASE("Stereo *operator", "[operator*]"){
    std::pair<int8_t, int8_t> p1, p2;
    std::pair<float, float> vf;
    p1 = {10, 20};
    p2 = {30, 40};
    vf = {0.5, 0.3};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a, c;
    a = audio<std::pair<int8_t, int8_t>>(v);
    c = a * vf;
    REQUIRE(c.get_buffer()[0].first == 5);
    REQUIRE(c.get_buffer()[0].second == 6);
    REQUIRE(c.get_buffer()[1].first == 15);
    REQUIRE(c.get_buffer()[1].second == 12);
  }

  TEST_CASE("Stereo +operator", "[operator+]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v1, v2;
    v1 = {p1};
    v2 = {p2};
    audio<std::pair<int8_t, int8_t>> a, b, c;
    a = audio<std::pair<int8_t, int8_t>>(v1);
    b = audio<std::pair<int8_t, int8_t>>(v2);
    c = a + b;
    REQUIRE(c.get_buffer()[0].first == 4);
    REQUIRE(c.get_buffer()[0].second == 6);
  }

  TEST_CASE("Stereo ^ operator", "[operator^]"){
    audio<std::pair<int8_t, int8_t>> a, b;
    std::pair<int, int> p = {1, 2};
    std::pair<int8_t, int8_t> p1, p2, p3;
    p1 = {1, 2};
    p2 = {3, 4};
    p3 = {4, 5};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2, p3};
    a = audio<std::pair<int8_t, int8_t>>(v);
    b = a ^ p;
    REQUIRE(b.get_buffer()[0].first == 1);
    REQUIRE(b.get_buffer()[0].second == 2);
  }

  TEST_CASE("Stereo Ranged add", "[Ranged Add]"){
    audio<std::pair<int8_t, int8_t>> s1, s2, s3;
    std::pair<int8_t, int8_t> p1, p2, p3;
    std::pair<int, int> r1, r2;
    r1 = {1, 2};
    r2 = {1, 2};
    p1 = {1, 2};
    p2 = {3, 4};
    p3 = {4, 5};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2, p3};
    s1 = s2 = audio<std::pair<int8_t, int8_t>>(v);
    s3 = s1.ranged_add(r1, r2, s2);
    REQUIRE(s3.get_buffer()[0].first == 6);
    REQUIRE(s3.get_buffer()[0].second == 8);
  }

  TEST_CASE("Stereo Calculate RMS", "[Calculate RMS]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a = audio<std::pair<int8_t, int8_t>>(v);
    std::pair<float, float> rms_pair = a.calculate_rms();
    REQUIRE(rms_pair.first == (float)sqrt(10.0 / 2));
    REQUIRE(rms_pair.second == (float)sqrt(20.0 / 2));
  }

  TEST_CASE("Stereo Normalize", "[Normalize]"){
    std::pair<int8_t, int8_t> p1, p2;
    p1 = {1, 2};
    p2 = {3, 4};
    std::vector<std::pair<int8_t, int8_t>> v = {p1, p2};
    audio<std::pair<int8_t, int8_t>> a, b;
    a = audio<std::pair<int8_t, int8_t>>(v);
    std::pair<float, float> rms_pair = a.calculate_rms();
    b = a.normalize(rms_pair, 2);
  }
