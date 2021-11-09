sequenceDiagram
    loop process_tasks
        loop process_commands
        Orchestrator->>DatasetTaskQueue:push(DatasetTask) 
        end
    
        WorkerThread->>DatasetTaskQueue: pop()
        DatasetTaskQueue-->>WorkerThread: DatasetTask
        WorkerThread-->>WorkerThread: DatasetTask.process()
    
        Orchestrator->>DatasetTaskQueue:pop() 
        DatasetTaskQueue-->>Orchestrator: DatasetTask
        Orchestrator-->>Orchestrator: DatasetTask.process()
    end

classDiagram
      Orchestrator *--  "n" WorkerThread
      direction LR
      Orchestrator -->  DatasetTaskResults
      Orchestrator --> DatasetTaskQueue
      WorkerThread --> DatasetTaskResults
      WorkerThread --> DatasetTaskQueue
      DatasetTaskQueue --> "0...*"DatasetTask
      DatasetTask --> Dataset
      DatasetTask --> DatasetTaskResults

      class Orchestrator{          
          -process_commands()
          +run();
      }
      class WorkerThread{          
          +process_tasks()
      }
      class DatasetTaskQueue{
          +push()
          +pop()
      }
      class DatasetTaskResults{
          +update(partial_result)
          +print()
      }
      class DatasetTask{
          operation
          +process()
      }
      class Dataset{
          filepath
          +sum(columnn)
          +max(columnn)
          +min(columnn)
      }