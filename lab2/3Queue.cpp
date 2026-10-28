
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int MAX_WAITING = 5;

// ================= CUSTOMER =================
class Customer
{
public:
    int id;
    int arrivalTime;
    int serviceTime;

    Customer(int i = 0, int arrival = 0, int service = 0)
        : id(i), arrivalTime(arrival), serviceTime(service) {}
};

// ================= DYNAMIC QUEUE =================
class Queue
{
private:
    Customer* data;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;

    void resize()
    {
        int newCapacity = capacity * 2;
        Customer* temp = new Customer[newCapacity];

        for (int i = 0; i < count; i++)
            temp[i] = data[(frontIndex + i) % capacity];

        delete[] data;
        data = temp;
        capacity = newCapacity;
        frontIndex = 0;
        rearIndex = count;
    }

public:
    Queue(int initialCapacity = 8)
        : capacity(initialCapacity), frontIndex(0),
          rearIndex(0), count(0)
    {
        data = new Customer[capacity];
    }

    ~Queue()
    {
        delete[] data;
    }

    // Prevent shallow-copy problems with dynamic memory
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const Customer& customer)
    {
        if (count == capacity)
            resize();

        data[rearIndex] = customer;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }

    Customer dequeue()
    {
        if (isEmpty())
            return Customer();

        Customer customer = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;

        return customer;
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    int size() const
    {
        return count;
    }
};

// ================= SERVICE COUNTER =================
class ServiceCounter
{
public:
    int counterId;
    Queue waitingQueue;

    bool busy;
    Customer currentCustomer;
    int finishTime;

    ServiceCounter(int id)
        : counterId(id), busy(false), finishTime(0) {}

    bool hasWaitingCustomers() const
    {
        return !waitingQueue.isEmpty();
    }

    void startNextCustomer(int currentTime)
    {
        if (!busy && !waitingQueue.isEmpty())
        {
            currentCustomer = waitingQueue.dequeue();
            busy = true;
            finishTime = currentTime + currentCustomer.serviceTime;
        }
    }
};

// ================= BANK SIMULATION =================
class BankSimulation
{
private:
    ServiceCounter** counters;
    int counterCount;
    int counterCapacity;

    int totalSimulationTime;
    int minArrivalInterval;
    int maxArrivalInterval;
    int minServiceTime;
    int maxServiceTime;

    int nextCustomerId;
    int totalArrived;
    int totalCompleted;
    int totalWaitingTime;

    ServiceCounter* createCounter(int id)
    {
        return new ServiceCounter(id);
    }

    void addCounter()
    {
        if (counterCount == counterCapacity)
        {
            int newCapacity = counterCapacity * 2;
            ServiceCounter** temp =
                new ServiceCounter*[newCapacity];

            for (int i = 0; i < counterCount; i++)
                temp[i] = counters[i];

            delete[] counters;
            counters = temp;
            counterCapacity = newCapacity;
        }

        counters[counterCount] = createCounter(counterCount + 1);
        counterCount++;

        cout << "Opened counter #" << counterCount << endl;
    }

    int randomBetween(int low, int high)
    {
        return low + rand() % (high - low + 1);
    }

    int findShortestQueue() const
    {
        int best = 0;

        for (int i = 1; i < counterCount; i++)
        {
            if (counters[i]->waitingQueue.size() <
                counters[best]->waitingQueue.size())
            {
                best = i;
            }
        }

        return best;
    }

    void assignCustomer(const Customer& customer)
    {
        int selected = findShortestQueue();

        // If the shortest queue already has 5 waiting customers,
        // the new customer must go to a newly opened counter.
        if (counters[selected]->waitingQueue.size() >= MAX_WAITING)
        {
            addCounter();
            selected = counterCount - 1;
        }

        counters[selected]->waitingQueue.enqueue(customer);

        cout << "Customer " << customer.id
             << " joined counter #" << counters[selected]->counterId
             << " (waiting: "
             << counters[selected]->waitingQueue.size()
             << ")" << endl;
    }

public:
    BankSimulation(int duration,
                   int minArrival, int maxArrival,
                   int minService, int maxService)
        : counterCount(0), counterCapacity(2),
          totalSimulationTime(duration),
          minArrivalInterval(minArrival),
          maxArrivalInterval(maxArrival),
          minServiceTime(minService),
          maxServiceTime(maxService),
          nextCustomerId(1), totalArrived(0),
          totalCompleted(0), totalWaitingTime(0)
    {
        counters = new ServiceCounter*[counterCapacity];
        addCounter();
    }

    ~BankSimulation()
    {
        for (int i = 0; i < counterCount; i++)
            delete counters[i];

        delete[] counters;
    }

    void run()
    {
        int nextArrival = 0;

        cout << "\n===== BANK SIMULATION STARTED =====\n";

        for (int currentTime = 0;
             currentTime < totalSimulationTime;
             currentTime++)
        {
            // Finish services
            for (int i = 0; i < counterCount; i++)
            {
                ServiceCounter* counter = counters[i];

                if (counter->busy &&
                    counter->finishTime <= currentTime)
                {
                    totalCompleted++;

                    cout << "Time " << currentTime
                         << ": Customer "
                         << counter->currentCustomer.id
                         << " completed at counter #"
                         << counter->counterId << endl;

                    counter->busy = false;
                }

                counter->startNextCustomer(currentTime);
            }

            // Customer arrival
            if (currentTime >= nextArrival)
            {
                int serviceTime =
                    randomBetween(minServiceTime, maxServiceTime);

                Customer customer(nextCustomerId++,
                                  currentTime, serviceTime);

                totalArrived++;
                assignCustomer(customer);

                nextArrival = currentTime +
                    randomBetween(minArrivalInterval,
                                  maxArrivalInterval);
            }
        }

        cout << "\n===== SIMULATION RESULTS =====\n";
        cout << "Simulation duration: "
             << totalSimulationTime << endl;
        cout << "Total customers arrived: "
             << totalArrived << endl;
        cout << "Customers completed: "
             << totalCompleted << endl;
        cout << "Customers not completed: "
             << totalArrived - totalCompleted << endl;
        cout << "Total service counters opened: "
             << counterCount << endl;

        cout << "\nFinal counter status:\n";

        for (int i = 0; i < counterCount; i++)
        {
            cout << "Counter #" << counters[i]->counterId
                 << " | Waiting: "
                 << counters[i]->waitingQueue.size()
                 << " | Busy: "
                 << (counters[i]->busy ? "Yes" : "No")
                 << endl;
        }
    }
};

// ================= MAIN =================
int main()
{
    srand(time(NULL));

    int duration;
    int minArrival, maxArrival;
    int minService, maxService;

    cout << "Enter simulation duration: ";
    cin >> duration;

    cout << "Enter minimum and maximum arrival interval: ";
    cin >> minArrival >> maxArrival;

    cout << "Enter minimum and maximum service time: ";
    cin >> minService >> maxService;

    if (duration <= 0 ||
        minArrival <= 0 || maxArrival < minArrival ||
        minService <= 0 || maxService < minService)
    {
        cout << "Invalid simulation settings!" << endl;
        return 1;
    }

    BankSimulation bank(duration, minArrival, maxArrival,
                        minService, maxService);

    bank.run();

    return 0;
}
