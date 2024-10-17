class Machine:
    total_Machines = 0

    def __init__(self):
        self.jobs = []
        Machine.total_Machines += 1
        self.num = Machine.total_Machines

    def print_jobs(self, finish_time):
        print(f"Machine{self.num}", end="  ")

        for t in range(finish_time+1):
            is_blank = True

            for job in self.jobs:
                if job[1][0] <= t and t < job[1][1]:
                    print(f"t{job[0]}", end="  ")
                    is_blank = False
            if is_blank:
                print("  ", end="  ")

def JobScheduling(input):
    running_machine = []
    finish_time = 0
    sorted_input = sorted(input, key=lambda input: input[1][0])

    while(len(sorted_input) > 0):
        input_job = sorted_input.pop(0)
        is_appended = False

        if(len(running_machine) > 0):
            for machine in running_machine:
                if machine.jobs[-1][1][1] <= input_job[1][0]:
                    machine.jobs.append(input_job)
                    is_appended = True
        if not is_appended:
            machine = Machine()
            machine.jobs.append(input_job)
            running_machine.append(machine)

        if(finish_time < input_job[1][1]):
            finish_time = input_job[1][1]

    return finish_time, running_machine

def main():
    input_job = []
    
    with open('input_Job.txt', 'r') as file:
        for idx, line in enumerate(file, start=1):
            pair = eval(line.strip())
            input_job.append((idx, pair))

    finish_time, result = JobScheduling(input_job)

    print("Time   ", end = "  ")
    for i in range (finish_time):
        print(f"{i:2d}", end="  ")
    print()
    
    for machine in result:
        machine.print_jobs(finish_time)
        print()
    
if __name__ == "__main__":
    main()