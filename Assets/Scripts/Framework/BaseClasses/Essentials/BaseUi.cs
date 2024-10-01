using UnityEngine;

public abstract class BaseUi : MonoBehaviour
{
    [HideInInspector] public BaseScene Scene { get; protected set; }
    [HideInInspector] public UiElement activeElement;

    public void Initialize(BaseScene sceneRef)
    {
        Scene = sceneRef;
        activeElement = null;
        InitializeMenus();
    }

    public abstract void InitializeMenus();

    public void MoveToMenu(UiMenu menu)
    {
        activeElement = menu;
        menu.Show();
    }
    public void GoBack()
    {
        activeElement.Hide();
        activeElement = null;
    }
}
