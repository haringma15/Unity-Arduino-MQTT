using UnityEngine;

public abstract class BaseScene : MonoBehaviour
{
    public BaseUi ui;

    void Start()
    {
        ui.Initialize(this);
        InitializeSpecifics();
    }

    public abstract void InitializeSpecifics();
}
