using UnityEngine.UI;

public class Prototype_MqttMenu : UiMenu
{
    public Text CounterText;
    private int counter = 0;

    public void IncrementCounter() => CounterText.text = $"{counter++}";


    public void OnClick_Test1()
    {
        DB.MqttClient.Publish(DB.Helpers.testTopic, "GREEN");
    }
    public void OnClick_Test2()
    {
        DB.MqttClient.Publish(DB.Helpers.testTopic, "RED");
    }
}
