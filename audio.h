#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <limits>

template<typename T>
class audio{
private:
  std::vector<T> mono;
  int sample_length;
public:
  audio(size_t dim = 0) : mono(std::vector<T>(dim)){}

  audio(std::vector<T> lst) : mono(lst.size()) {
		copy(lst.begin(), lst.end(), mono.begin());
	}

  audio(std::vector<T> lst, int sampl_len) : mono(lst.size()), sample_length(sampl_len){
    copy(lst.begin(), lst.end(), mono.begin());
  }

  virtual ~audio() = default;

  audio(const audio& rhs) = default;

  audio& operator=(const audio& rhs) = default;

  audio(audio&& rhs) : mono(std::move(rhs.mono)){}

  T& operator[] (std::size_t index){
    return mono[index];
  }

  audio operator=(audio&& rhs){
    if (this != &rhs){
      this->mono = move(rhs.mono);
      return *this;
    }
  }

  std::vector<T> get_buffer() const{
    return mono;
  }

  int get_sample_length() const{
    return sample_length;
  }

  audio operator|(const audio& rhs){
    auto temporary_audio = *this;
    for(auto& value : rhs.mono){
			temporary_audio.mono.push_back(value);
		}
    return temporary_audio;
  }

  audio operator*(const std::pair<float, float>& volume_factor){
    auto temporary_audio = *this;
    for(int i = 0; i < this->mono.size(); ++i){
      temporary_audio.mono[i] = this->mono[i] * volume_factor.first;
		}
    return temporary_audio;
  }

  audio operator+(const audio& rhs){
    auto temporary_audio = *this;
    for(int i = 0; i < this->mono.size(); ++i){
       int sum = this->mono[i] + rhs.mono[i];
      if(sum >= std::numeric_limits<T>::max()){
        temporary_audio.mono[i] = (T)std::numeric_limits<T>::max();
      }
      else{
        temporary_audio.mono[i] = sum;
      }
    }
    return temporary_audio;
  }

  audio operator^(const std::pair<int, int>& range){
    auto temporary_audio = *this;
    temporary_audio.mono.clear();
    for(int i = 0; i < this->mono.size(); ++i){
        if(i < range.first || i > range.second){
          temporary_audio.mono.push_back(this->mono[i]);
        }
    }
    return temporary_audio;
  }

  void reverse (){
   std::reverse(mono.begin(), mono.end());
  }

  audio ranged_add(const std::pair<int, int>& range1, const std::pair<int, int>& range2, const audio& rhs){
    auto temporary_audio = *this;
    auto temporary_rhs = rhs;
    audio<T> result;
    temporary_rhs.mono = temporary_audio.mono = std::vector<T>(range1.second - range1.first + 1);
    std::copy(this->mono.begin() + range1.first, this->mono.begin() + range1.second + 1, temporary_audio.mono.begin());
    std::copy(rhs.mono.begin() + range2.first, rhs.mono.begin() + range2.second + 1, temporary_rhs.mono.begin());

    result = temporary_audio + temporary_rhs;
    return result;
  }

  float calculate_rms(){
    int init = 0;
    float sum = std::accumulate(this->mono.begin(),
                                this->mono.end(),
                                init, [&](T accumulated_sum, T x){return(accumulated_sum + pow(x, 2.0));});
    return sqrt(sum / this->mono.size());
  }

  audio normalize(float current_rms, float desired_rms){
    auto temporary_audio = *this;
    std::transform(this->mono.begin(),
                   this->mono.end(),
                   temporary_audio.mono.begin(),
                   [&](T x){return (x *= (desired_rms / current_rms));}
                 );

    return temporary_audio;
  }

  audio fade_in(int number_of_seconds){
    int fade_sample_no = 1;
    float ramp_length = number_of_seconds * sample_length;
    auto temporary_audio = *this;

    std::for_each(temporary_audio.mono.begin(),
                  temporary_audio.mono.end(),
                  [&fade_sample_no, ramp_length](T x){
                    while(fade_sample_no < ramp_length){
                      x = (fade_sample_no / ramp_length) * x;
                      ++fade_sample_no;
                    }
                  }
                );

    return temporary_audio;
  }

  audio fade_out(int number_of_seconds){
    int fade_sample_no = 1;
    float ramp_length = number_of_seconds * sample_length;
    auto temporary_audio = *this;

    std::for_each(temporary_audio.mono.begin(),
                  temporary_audio.mono.end(),
                  [&fade_sample_no, ramp_length](T x){
                    while(fade_sample_no < ramp_length){
                      x = (1 - fade_sample_no / ramp_length) * x;
                      ++fade_sample_no;
                    }
                  }
                );

    return temporary_audio;
  }
};

template<typename T>
class audio<std::pair<T, T>>{
private:
  std::vector<std::pair<T, T>> stereo;
  int sample_length;
public:
  audio(size_t dim = 0) : stereo(std::vector<std::pair<T, T>>(dim)){}

  audio(std::vector<std::pair<T, T>> lst) : stereo(lst.size()) {
		copy(lst.begin(), lst.end(), stereo.begin());
	}

  audio(std::vector<std::pair<T, T>> lst, int sampl_len) : stereo(lst.size()), sample_length(sampl_len){
    copy(lst.begin(), lst.end(), stereo.begin());
  }

  virtual ~audio() = default;

  audio(const audio<std::pair<T, T>>& rhs) = default;

  audio<std::pair<T, T>>& operator=(const audio<std::pair<T, T>>& rhs) = default;

  int get_sample_length() const{
    return sample_length;
  }

  std::vector<std::pair<T, T>> get_buffer() const{
    return stereo;
  }

  audio(audio<std::pair<T, T>>&& rhs) : stereo(std::move(rhs.stereo)){}

  audio<std::pair<T, T>> operator=(audio<std::pair<T, T>>&& rhs){
    if (this != &rhs){
      this->stereo = move(rhs.stereo);
      return *this;
    }
  }

  audio<std::pair<T, T>> operator|(const audio<std::pair<T, T>>& rhs){
    auto temporary_audio = *this;
    for(auto& value : rhs.stereo){
      temporary_audio.stereo.push_back(value);
    }
    return temporary_audio;
  }

  audio<std::pair<T, T>> operator*(const std::pair<float, float>& volume_factor){
    auto temporary_audio = *this;
    for(auto& value : temporary_audio.stereo){
       value.first *= volume_factor.first;
       value.second *= volume_factor.second;
    }
    return temporary_audio;
  }

  audio<std::pair<T, T>> operator+(const audio<std::pair<T, T>>& rhs){
    auto temporary_audio = *this;
    for(int i = 0; i < temporary_audio.stereo.size(); i++){
      int sum1 = this->stereo[i].first + rhs.stereo[i].first;
      if(sum1 >= std::numeric_limits<T>::max()){
        temporary_audio.stereo[i].first = (T)std::numeric_limits<T>::max();
      }
      else{
        temporary_audio.stereo[i].first = sum1;
      }
      int sum2 = this->stereo[i].second + rhs.stereo[i].second;
      if(sum2 >= std::numeric_limits<T>::max()){
        temporary_audio.stereo[i].second = (T)std::numeric_limits<T>::max();
      }
      else{
        temporary_audio.stereo[i].second = sum2;
      }
    }
    return temporary_audio;
  }

  audio<std::pair<T, T>> operator^(const std::pair<int, int>& range){
    auto temporary_audio = *this;
    temporary_audio.stereo.clear();
    for(int i = 0; i < this->stereo.size(); ++i){
        if(i < range.first || i > range.second){
          temporary_audio.stereo.push_back(this->stereo[i]);
        }
    }
    return temporary_audio;
  }

  void reverse (){
   std::reverse(stereo.begin(), stereo.end());
  }

  audio<std::pair<T, T>> ranged_add(const std::pair<int, int>& range1, const std::pair<int, int>& range2, const audio<std::pair<T, T>>& rhs){
    auto temporary_audio = *this;
    auto temporary_rhs = rhs;
    audio<std::pair<T, T>> result;
    temporary_rhs.stereo = temporary_audio.stereo = std::vector<std::pair<T, T>>(range1.second - range1.first + 1);
    std::copy(this->stereo.begin() + range1.first, this->stereo.begin() + range1.second + 1, temporary_audio.stereo.begin());
    std::copy(rhs.stereo.begin() + range2.first, rhs.stereo.begin() + range2.second + 1, temporary_rhs.stereo.begin());

    result = temporary_audio + temporary_rhs;
    return result;
  }

  std::pair<float, float> calculate_rms(){
    int init = 0;
    float rms1, rms2;
    rms1 = std::accumulate(this->stereo.begin(),
                                this->stereo.end(),
                                init, [&](T accumulated_sum, std::pair<T, T> x){return(accumulated_sum + pow(x.first, 2.0));});
    rms1 = (float)sqrt(rms1 / this->stereo.size());

    rms2 = std::accumulate(this->stereo.begin(),
                                this->stereo.end(),
                                init, [&](T accumulated_sum, std::pair<T, T> x){return(accumulated_sum + pow(x.second, 2.0));});

    rms2 = (float)sqrt(rms2 / this->stereo.size());

    std::pair<float, float> rms_pair = {rms1, rms2};
    return rms_pair;
  }

  audio<std::pair<T, T>> normalize(std::pair<float, float> rms_pair, float desired_rms){
    auto temporary_audio = *this;

    std::transform(this->stereo.begin(),
                   this->stereo.end(),
                   temporary_audio.stereo.begin(),
                   [&](std::pair<T, T> x){return (x.first *= (desired_rms / rms_pair.first));}
                 );

   std::transform(this->stereo.begin(),
                  this->stereo.end(),
                  temporary_audio.stereo.begin(),
                  [&](std::pair<T, T> x){return (x.second *= (desired_rms / rms_pair.second));}
                );

    return temporary_audio;
  }

  audio<std::pair<T, T>> fade_in(int number_of_seconds){
    int fade_sample_no = 1;
    float ramp_length = number_of_seconds * sample_length;
    auto temporary_audio = *this;

    std::for_each(temporary_audio.stereo.begin(),
                  temporary_audio.stereo.end(),
                  [&fade_sample_no, ramp_length](std::pair<T, T> x){
                    while(fade_sample_no < ramp_length){
                      x.first = (fade_sample_no / ramp_length) * x.first;
                      x.second = (fade_sample_no / ramp_length) * x.second;
                      ++fade_sample_no;
                    }
                  }
                );
    return temporary_audio;
  }

  audio<std::pair<T, T>> fade_out(int number_of_seconds){
    int fade_sample_no = 1;
    float ramp_length = number_of_seconds * sample_length;
    auto temporary_audio = *this;

    std::for_each(temporary_audio.stereo.begin(),
                  temporary_audio.stereo.end(),
                  [&fade_sample_no, ramp_length](std::pair<T, T> x){
                    while(fade_sample_no < ramp_length){
                      x.first = (1 - fade_sample_no / ramp_length) * x.first;
                      x.second = (1 - fade_sample_no / ramp_length) * x.second;
                      ++fade_sample_no;
                    }
                  }
                );
    return temporary_audio;
  }

};

#endif
