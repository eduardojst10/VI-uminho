#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    // resolution from the camera
    this->cam->getResolution(&W,&H);
    // number of samples per pixel
    const int spp = 16;

    // Create a vector of threads
    int num_threads = std::thread::hardware_concurrency(); // get the number of hardware threads
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // counter for processed rows
    std::atomic<int> rowsProcessed(0);

    // create a lambda function to be executed by each thread
    auto worker = [&](int start, int end) {
        // Create a thread-local random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0, 1.0);

        for (int y = start ; y< end ; y++) {  // loop over rows
            for (int x=0 ; x< W ; x++) { // loop over columns
                Ray primary;
                Intersection isect;
                bool intersected = false;
                RGB color(0.f, 0.f,0.f);
                //iterate through the pixel samples
                for (int ss = 0; ss < spp; ss++){
                    RGB this_color;
                    // taking multiple random samples within each pixel and averaging their color contributions
                    float jitterV[2] = {dis(gen), dis(gen)};
                    // Generate Ray (camera)
                    this->cam->GenerateRay(x,y,&primary,jitterV);

                    // trace ray (scene), type of intersection
                    intersected = this->scene->trace(primary,&isect);

                    // shade this intersection (shader)
                    this_color = this->shd->shade(intersected,isect,0);
                    color += this_color;
                }
                color = color / spp;
                // write the result into the image frame buffer (image)
                img->set(x,y,color);
            }
            ++rowsProcessed; // increment the count of processed rows
        }
    };

    // Create a thread for the progress bar
    std::thread progressBarThread([&]() {
        while (rowsProcessed < H) {
            float progress = static_cast<float>(rowsProcessed) / H;
            int pos = static_cast<int>(progress * 40);
            std::cout << "[";
            for (int i = 0; i < 40; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << "%\r";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // divide the work among the threads
    int rows_per_thread = H / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start_row = i * rows_per_thread;
        // make the last thread handle any extra rows
        int end_row = (i == num_threads - 1) ? H : start_row + rows_per_thread;
        threads.emplace_back(worker, start_row, end_row);
    }

    // join the threads (wait for them to complete)
    for (auto& thread : threads) {
        thread.join();
    }

    // Update the progress to 100%
    std::cout << "[";
    for (int i = 0; i < 40; ++i) {
        std::cout << "=";
    }
    std::cout << "] " << 100 << "%\r";
    std::cout.flush();

    // join the progress bar thread
    progressBarThread.join();

    std::cout << std::endl;
}
