#include <iostream>
#include <future>
#include <chrono>

using namespace std::chrono;


// simulated callback from mdc or pin
void mySyncFunc(std::promise<bool> promise){
    std::this_thread::sleep_for(9000ms);
    promise.set_value(true);
}
int main()
{   
    // create a promise
    std::promise<bool> ready_promise;
    // get the future from promise
    std::shared_future<bool> ready_future(ready_promise.get_future());
 

    // function to invoke mdc and sync feedback
    auto fun1 = [&, ready_future]() -> bool 
    {
        //ready_future.wait();
        auto status = ready_future.wait_for(10000ms);// waits for the signal set in simulated callback
        if(status == std::future_status::ready)
            return ready_future.get();
        else if(status == std::future_status::timeout || status == std::future_status::deferred) 
            return false;
        else 
            return false;
    };
 
    // call sync func async
    auto result1 = std::async(std::launch::async, fun1);

    // simulate a callback setting the promise
    mySyncFunc(std::move(ready_promise));
 
    // check result
    std::cout << "Thread 1 received the signal "
              << result1.get() << "\n";

}