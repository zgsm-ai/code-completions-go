import java.util.ArrayList;
import java.util.List;

public class TodoList {
    private List<String> tasks;
    
    public TodoList() {
        <｜fim▁hole｜>tasks = new ArrayList<>();
    }
    
    public void addTask(String task) {
        tasks.add(task);
        System.out.println("Added: " + task);
    }
    
    public void displayTasks() {
        System.out.println("Todo List:");
        for (int i = 0; i < tasks.size(); i++) {
            System.out.println("  " + (i + 1) + ". " + tasks.get(i));
        }
    }
    
    public static void main(String[] args) {
        TodoList todoList = new TodoList();
        todoList.addTask("Buy groceries");
        todoList.addTask("Finish homework");
        todoList.displayTasks();
    }
}