
#include <iostream>
#include <string>
using namespace std;

struct PrintJob {
    int jobID;
    string employeeName;
    string documentName;
    int numberOfPages;
    int submissionTime;
};

// Node for the priority queue and failed-jobs stack.
struct Node {
    PrintJob job;
    Node* next;

    Node(PrintJob j) {
        job = j;
        next = NULL;
    }
};

class PrinterSystem {
private:
    // Priority Queue implemented using a singly linked list.
    Node* front;

    // Stack of failed jobs implemented using a singly linked list.
    Node* top;

    int nextJobID;
    int nextSubmissionTime;

    int completedJobs;
    int failedJobs;
    int cancelledJobs;

    // Insert a job according to SJF.
    // Jobs with equal page counts retain FIFO order.
    void insertPriority(PrintJob job) {
        Node* newNode = new Node(job);

        if (front == NULL ||
            job.numberOfPages < front->job.numberOfPages) {
            newNode->next = front;
            front = newNode;
            return;
        }

        Node* current = front;

        while (current->next != NULL &&
               current->next->job.numberOfPages
                   <= job.numberOfPages) {
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    // Push a failed job onto the stack.
    void pushFailed(PrintJob job) {
        Node* newNode = new Node(job);
        newNode->next = top;
        top = newNode;
    }

    // Remove and return the top failed job.
    PrintJob popFailed() {
        Node* temp = top;
        PrintJob job = temp->job;

        top = top->next;
        delete temp;

        return job;
    }

    void displayJob(PrintJob job) {
        cout << "Job ID: " << job.jobID << '\n';
        cout << "Employee: " << job.employeeName << '\n';
        cout << "Document: " << job.documentName << '\n';
        cout << "Pages: " << job.numberOfPages << '\n';
        cout << "Submission Time: " << job.submissionTime << '\n';
    }

public:
    PrinterSystem() {
        front = NULL;
        top = NULL;

        nextJobID = 101;
        nextSubmissionTime = 1;

        completedJobs = 0;
        failedJobs = 0;
        cancelledJobs = 0;
    }

    // Prevent shallow copying of linked-list pointers.
    PrinterSystem(const PrinterSystem&);
    PrinterSystem& operator=(const PrinterSystem&);

    ~PrinterSystem() {
        Node* current;

        while (front != NULL) {
            current = front;
            front = front->next;
            delete current;
        }

        while (top != NULL) {
            current = top;
            top = top->next;
            delete current;
        }
    }

    // 1. Submit a new print job.
    void submitPrintJob() {
        PrintJob job;

        job.jobID = nextJobID++;

        cout << "Enter employee name: ";
        cin >> ws;
        getline(cin, job.employeeName);

        cout << "Enter document name: ";
        getline(cin, job.documentName);

        cout << "Enter number of pages: ";
        if (!(cin >> job.numberOfPages) ||
            job.numberOfPages <= 0) {
            cout << "Invalid page count.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            return;
        }

        job.submissionTime = nextSubmissionTime++;

        insertPriority(job);

        cout << "Print job submitted successfully.\n";
        cout << "Assigned Job ID: " << job.jobID << '\n';
    }

    // 2. Process the job with the fewest pages.
    void processPrintJob() {
        if (front == NULL) {
            cout << "No pending jobs to print.\n";
            return;
        }

        Node* temp = front;
        PrintJob job = temp->job;

        front = front->next;
        delete temp;

        cout << "\nPrinting job...\n";
        displayJob(job);

        // Simulate successful printing.
        completedJobs++;

        cout << "Printing completed successfully.\n";
    }

    // 3. Display pending jobs in SJF processing order.
    void displayPendingJobs() {
        if (front == NULL) {
            cout << "No pending jobs.\n";
            return;
        }

        cout << "\n===== PENDING PRINT JOBS (SJF) =====\n";

        Node* current = front;

        while (current != NULL) {
            cout << "ID: " << current->job.jobID
                 << " | Employee: " << current->job.employeeName
                 << " | Document: " << current->job.documentName
                 << " | Pages: " << current->job.numberOfPages
                 << " | Time: " << current->job.submissionTime
                 << '\n';

            current = current->next;
        }
    }

    // 4. Cancel a pending job by its ID.
    void cancelPrintJob() {
        if (front == NULL) {
            cout << "No pending jobs to cancel.\n";
            return;
        }

        int id;
        cout << "Enter Job ID to cancel: ";

        if (!(cin >> id)) {
            cout << "Invalid Job ID.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            return;
        }

        Node* current = front;
        Node* previous = NULL;

        while (current != NULL &&
               current->job.jobID != id) {
            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            cout << "Pending job not found.\n";
            return;
        }

        if (previous == NULL)
            front = current->next;
        else
            previous->next = current->next;

        cout << "Cancelled job: "
             << current->job.documentName << '\n';

        delete current;
        cancelledJobs++;
    }

    // 5. Simulate a failure while processing the next job.
    void simulatePrinterFailure() {
        if (front == NULL) {
            cout << "No pending jobs to fail.\n";
            return;
        }

        Node* temp = front;
        PrintJob job = temp->job;

        front = front->next;
        delete temp;

        pushFailed(job);
        failedJobs++;

        cout << "Printer failure simulated!\n";
        cout << "Job moved to failed-jobs stack:\n";
        displayJob(job);
    }

    // 6. Retry the most recently failed job.
    void retryFailedJob() {
        if (top == NULL) {
            cout << "No failed jobs to retry.\n";
            return;
        }

        PrintJob job = popFailed();

        // Reinsert according to SJF, preserving original
        // submission time for FIFO ties.
        insertPriority(job);

        failedJobs--;

        cout << "Job " << job.jobID
             << " returned to the pending queue.\n";
    }

    // 7. Display failed jobs from newest failure to oldest.
    void displayFailedJobs() {
        if (top == NULL) {
            cout << "No failed jobs.\n";
            return;
        }

        cout << "\n===== FAILED JOBS (STACK) =====\n";

        Node* current = top;

        while (current != NULL) {
            cout << "ID: " << current->job.jobID
                 << " | Employee: " << current->job.employeeName
                 << " | Document: " << current->job.documentName
                 << " | Pages: " << current->job.numberOfPages
                 << '\n';

            current = current->next;
        }
    }

    // 8. Calculate total pages waiting to be printed.
    int calculateTotalPages() {
        int total = 0;
        Node* current = front;

        while (current != NULL) {
            total += current->job.numberOfPages;
            current = current->next;
        }

        return total;
    }

    // 9. Display system statistics.
    void displayStatistics() {
        int pending = 0;
        Node* current = front;

        while (current != NULL) {
            pending++;
            current = current->next;
        }

        int failedCount = 0;
        current = top;

        while (current != NULL) {
            failedCount++;
            current = current->next;
        }

        cout << "\n===== PRINTER STATISTICS =====\n";
        cout << "Completed jobs: " << completedJobs << '\n';
        cout << "Failed jobs: " << failedCount << '\n';
        cout << "Pending jobs: " << pending << '\n';
        cout << "Cancelled jobs: " << cancelledJobs << '\n';
        cout << "Total pending pages: "
             << calculateTotalPages() << '\n';
    }
};

int main() {
    PrinterSystem printer;
    int choice;

    do {
        cout << "\n===== PRINTER MANAGEMENT =====\n";
        cout << "1. Submit Print Job\n";
        cout << "2. Process Print Job\n";
        cout << "3. Display Pending Jobs\n";
        cout << "4. Cancel Print Job\n";
        cout << "5. Simulate Printer Failure\n";
        cout << "6. Retry Failed Job\n";
        cout << "7. Display Failed Jobs\n";
        cout << "8. Calculate Total Pending Pages\n";
        cout << "9. Display Statistics\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printer.submitPrintJob();
            break;

        case 2:
            printer.processPrintJob();
            break;

        case 3:
            printer.displayPendingJobs();
            break;

        case 4:
            printer.cancelPrintJob();
            break;

        case 5:
            printer.simulatePrinterFailure();
            break;

        case 6:
            printer.retryFailedJob();
            break;

        case 7:
            printer.displayFailedJobs();
            break;

        case 8:
            cout << "Total pending pages: "
                 << printer.calculateTotalPages() << '\n';
            break;

        case 9:
            printer.displayStatistics();
            break;

        case 0:
            cout << "Exiting Printer Management System.\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
