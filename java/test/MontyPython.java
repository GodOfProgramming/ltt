import java.lang.reflect.Field;
public class MontyPython {
    public static void main(String[] args) throws Exception {
        Field v = Integer.class.getDeclaredField("value");
        v.setAccessible(true);
        v.set(3,5);
        System.out.printf("Ready the holy hand grenade!\n");
        Thread.sleep(1000);
        System.out.printf("%d\n", 1);
        Thread.sleep(1000);
        System.out.printf("%d\n", 2);
        Thread.sleep(1000);
        System.out.printf("%d\n", 3);
    }
}
