using UnityEngine;

public class UiElement : MonoBehaviour
{
    [HideInInspector] public BaseUi UI { get; protected set; }

    public void Initialize(BaseUi uiRef)
    {
        UI = uiRef;
        Hide();
    }


    public void Show() => gameObject.SetActive(true);
    public void Hide() => gameObject.SetActive(false);
}
