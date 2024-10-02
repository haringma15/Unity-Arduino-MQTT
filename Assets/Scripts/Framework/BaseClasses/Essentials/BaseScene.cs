using UnityEngine;

public abstract class BaseScene : MonoBehaviour
{
    public BaseUi ui;

    void Start()
    {
        DB.Helpers.currentScene = this;
        ui.Initialize(this);
        InitializeSpecifics();
    }

    public abstract void InitializeSpecifics();
}
