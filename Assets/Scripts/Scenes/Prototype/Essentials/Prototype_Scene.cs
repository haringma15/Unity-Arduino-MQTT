public class Prototype_Scene : BaseScene
{
    public override void InitializeSpecifics() {}

    public void IncrementCounter() => ((Prototype_UI)ui).IncrementCounter();
}
