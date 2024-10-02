public class Prototype_UI : BaseUi
{
    public Prototype_MqttMenu mqttMenu;

    public override void InitializeMenus()
    {
        mqttMenu.Initialize(this);
        mqttMenu.Show();
    }


    public void IncrementCounter() => mqttMenu.IncrementCounter();
}
