using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GUICLR;

namespace VisualScripting
{
    public class TestComp : Component
    {

        
        public TestComp() : base("Test Component")
        {
            AddSlider("Slider 1", 0.5f, 0.0f, 1.0f);
            AddSlider("Slider 2", 0.2f, 0.0f, 1.0f);
        }

        public override Guid ElementGuid => new Guid("733d497c-d74e-4b86-adde-86b61c45b87a");
    }

    public class Slider : Element
    {

        private GUICLR.Rectangle baseRectangle;
        private GUICLR.Rectangle slideRectangle;
        private GUICLR.Text title;
        private GUICLR.Text valueText;
        public Slider(string title, float currentValue, float maxValue, float minValue) : base(new CanvasPoint(0, 0), new CanvasSize(100, 100))
        {
            Title = title;
            CurrentValue = currentValue;
            MaxValue = maxValue;
            MinValue = minValue;

            setupBaseRendering();
        }

        private void setupBaseRendering()
        {
            baseRectangle = new GUICLR.Rectangle(Position.ToVector(), Size.ToVector(), 0.0f, 2.0f, 2.0f, 0.0f, Pivot.TOP_LEFT, Color.FromArgb(60, 60, 60));
            baseRectangle.setZDepth(0.0001f);

            float width = (float)Size.Width * (CurrentValue - MinValue) / (MaxValue - MinValue);
            slideRectangle = new GUICLR.Rectangle(Position.ToVector(), new CanvasSize(width, Size.Height).ToVector(), 0.0f, 2.0f, 2.0f, 0.0f, Pivot.TOP_LEFT, Color.FromArgb(52, 186, 235));
            slideRectangle.setZDepth(0.0002f);

            title = new GUICLR.Text(Title + " :", Position.ToVector(), Size.ToVector(), (float)Size.Height, TextAlignment.LEFT, Pivot.TOP_LEFT);
            title.setZDepth(0.0003f);

            valueText = new GUICLR.Text(CurrentValue.ToString(), Position.ToVector(), Size.ToVector(), (float)Size.Height, TextAlignment.RIGHT, Pivot.TOP_LEFT);
            valueText.setZDepth(0.0003f);
        }

        public string Title { get; set; }
        public float CurrentValue { get; set; }
        public float MaxValue { get; set; }
        public float MinValue { get; set; }

        public override CanvasSize Size
        {
            get
            {
                return base.Size;
            }

            set
            {
                base.Size = value;

                baseRectangle.setSize(base.Size.ToVector());

                float width = (float)Size.Width * (CurrentValue - MinValue) / (MaxValue - MinValue);
                slideRectangle.setSize(new CanvasSize(width, base.Size.Height).ToVector());

                title.setSize(new CanvasSize(base.Size.Width, base.Size.Height - 3.0f).ToVector());
                title.setFontSize((float)base.Size.Height - 3.0f);

                valueText.setSize(new CanvasSize(base.Size.Width - 2.0f, base.Size.Height - 3.0f).ToVector());
                valueText.setFontSize((float)base.Size.Height - 3.0f);
            }
        }

        public override CanvasPoint Position
        {
            get
            {
                return base.Position;
            }

            set
            {
                base.Position = value;

                baseRectangle.setPosition(base.Position.ToVector());

                slideRectangle.setPosition(base.Position.ToVector());

                title.setPosition(new CanvasPoint(base.Position.X + 2.0f, base.Position.Y).ToVector());

                valueText.setPosition(base.Position.ToVector());
            }
        }

        public override Guid ElementGuid => new Guid("733d497c-d73e-4b86-adde-86b61c45b87a");
    }
    

    public abstract class Component : Element
    {
        private string title;
        private GUICLR.Rectangle backPlane;
        private GUICLR.Rectangle accentColor;
        private GUICLR.Text titleText;
        private Color color;

        private float nextHeight;

        private List<Element> childComponents = new List<Element>();

        public Component(string _title) : base(new CanvasPoint(0, 0), new CanvasSize(100, 100))
        {
            color = Color.FromArgb(255, 103, 0);
            title = _title;

            this._instanceGuid = Guid.NewGuid();

            setupBaseRendering();
        }

        public Component(string _title, Color _color) : base(new CanvasPoint(0,0), new CanvasSize(100,100))
        {
            color = _color;
            title = _title;

            this._instanceGuid = Guid.NewGuid();

            setupBaseRendering();
        }

        public override void Translate(CanvasPoint translate)
        {
            Position += translate;

            foreach(Element elem in childComponents)
            {
                elem.Translate(translate);
            }
        }

        private void setupBaseRendering()
        {
            backPlane = new GUICLR.Rectangle(Position.ToVector(), Size.ToVector(), 2.0f, 2.0f, 2.0f, 2.0f, Pivot.TOP_LEFT, Color.Gray);
            backPlane.setZDepth(0.0000f);

            accentColor = new GUICLR.Rectangle(Position.ToVector(), new CanvasSize(Size.Width, 5.0f).ToVector(), 2.0f, 2.0f, 0.0f, 0.0f, Pivot.TOP_LEFT, Color);
            accentColor.setZDepth(0.0001f);

            titleText = new GUICLR.Text(title, new CanvasPoint(Position.X, Position.Y + 7.0f).ToVector(), new CanvasSize(Size.Width, 10.0f).ToVector(), 10.0f, TextAlignment.CENTER, Pivot.TOP_LEFT);
            titleText.setZDepth(0.0001f);

            nextHeight = 30.0f;

        }

        public void AddSlider(string _title, float currentValue, float maxValue, float minValue)
        {
            Slider slider = new Slider(_title, currentValue, maxValue, minValue);
            slider.Position = new CanvasPoint(0, nextHeight);
            slider.Size = new CanvasSize(Size.Width - 5.0f, 12.0f);

            childComponents.Add(slider);

            nextHeight += 12.0f + 4.0f;
        }

        public Color Color
        {
            get
            {
                return color;
            }
            set
            {
                color = value;
                accentColor.setColor(color);
            }
        }

        public override CanvasSize Size
        {
            get
            {
                return base.Size;
            }
            set
            {
                base.Size = value;

                backPlane.setSize(base.Size.ToVector());

                accentColor.setSize(new CanvasSize(base.Size.Width, 5.0f).ToVector());

                titleText.setSize(new CanvasSize(base.Size.Width, 10.0f).ToVector());
            }
        }

        public override CanvasPoint Position
        {
            get
            {
                return base.Position;
            }
            set
            {
                base.Position = value;

                backPlane.setPosition(base.Position.ToVector());

                accentColor.setPosition(base.Position.ToVector());

                titleText.setPosition(new CanvasPoint(base.Position.X, base.Position.Y + 7.0f).ToVector());
            }
        }

        public abstract override Guid ElementGuid { get; }
    }

    public abstract class Element : IDisposable
    {
        protected Guid _instanceGuid;
        protected VisualScripting.BoundingBox boundingBox;

        public virtual CanvasSize Size
        {
            get
            {
                return boundingBox.Size;
            }

            set
            {
                boundingBox.Size = value;
            }
        }

        public virtual CanvasPoint Position
        {
            get
            {
                return boundingBox.Location;
            }

            set
            {
                boundingBox.Location = value;
            }
        }

        public virtual void Translate(CanvasPoint translate)
        {
            Position += translate;
        }

        public Element(CanvasPoint position, CanvasSize size)
        {
            //_component = new GUICLR.Component("Element"); //!!!!!!!!!!!!!!!!!!!!!!!
            //_boundingBox = _component.getBoundingBox(); //!!!!!!!!!!!!!!!!!!!!!!!
            this._instanceGuid = Guid.NewGuid();
            boundingBox = new BoundingBox(position, size);

            Children = new ComponentCollection(this);
        }        

        public VisualScripting.BoundingBox Bounds //!!!!!!!!!!!!!!!!!!!!!!!
        {
            get
            {
                return this.boundingBox;
                //return VisualScripting.BoundingBox.FromLTRB((double)this._boundingBox.getX(), (double)this._boundingBox.getY(), (double)this._boundingBox.getWidth(), (double)this._boundingBox.getHeight());
            }
        }
        public Guid InstanceGuid { get => _instanceGuid; }
        public ComponentCollection Children { get; }
        public string Name { get; set; }
        public string ElementType { get; set; }
        public abstract Guid ElementGuid { get; }

        public static bool operator == (Element A, Element B)
        {
            return (A.InstanceGuid == B.InstanceGuid);
        }
        public static bool operator != (Element A, Element B)
        {
            return (A.InstanceGuid != B.InstanceGuid);
        }
        public override bool Equals(object obj)
        {
            if (!(obj is Element)) return false;
            Element c = obj as Element;
            return (this.InstanceGuid == c.InstanceGuid);
        }
        public bool Equals(Element other)
        {
            return (this.InstanceGuid == other.InstanceGuid);
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        public override string ToString()
        {
            return this.Name;
        }
        public void Dispose()
        {
            //_component.Dispose();
        }
        ~Element()
        {
            this.Dispose();
        }
    }

    //TODO: Fix enumerator implementation
    public class ComponentCollection : IList<Element>, ICollection<Element>, IEnumerable<Element>
    {
        private Element[] _components = new Element[1];
        private Element _parent;
        private bool _readOnly;

        public ComponentCollection() : base()
        {
            this._parent = null;
            this._readOnly = false;
        }
        public ComponentCollection(Element owner) : base()
        {
            this._parent = owner;
            this._readOnly = false;
        }

        public Element this[int index]
        {
            get
            {
                return this._components[index];
            }
            set
            {
                if (index < 0) throw new IndexOutOfRangeException("Index cannot be <0.");
                if (this._components.Length > index)
                {
                    this._components[index] = value;
                }
                else throw new IndexOutOfRangeException($"Index {index} is out of range [0 - {(this._components.Length - 1)}].");
            }
        }

        public Element Owner => this._parent;
        public int Count => _components.Length;

        public bool IsReadOnly => _readOnly;
        public void Add(Element item)
        {
            this._components.Append<Element>(item);
        }
        public void Clear()
        {
            _components = new Element[] { };
        }
        public bool Contains(Element item)
        {
            for (int i = 0; i < this._components.Length; i++)
            {
                if (item == this._components[i])
                {
                    return true;
                }
            }
            return false;
        }
        public void CopyTo(Element[] array, int arrayIndex)
        {
            this._components.CopyTo(array, arrayIndex);
        }
        public IEnumerator<Element> GetEnumerator()
        {
            foreach (Element element in _components)
            {
                yield return element;
            }
        }
        public int IndexOf(Element item)
        {
            for (int i = 0; i < this._components.Length; i++)
            {
                if (item == this._components[i])
                {
                    return i;
                }
            }
            return -1;
        }
        public void Insert(int index, Element item)
        {
            int n = this._components.Length + 1;
            Element[] newarr = new Element[n];
            for (int i = 0; i < n + 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._components[i];
                else if (i == index - 1)
                    newarr[i] = item;
                else
                    newarr[i] = this._components[i - 1];
            }
            this._components = newarr;
        }
        public bool Remove(Element item)
        {
            int i = this.IndexOf(item);
            if (i != -1)
            {
                this.RemoveAt(i);
                return true;
            }
            else return false;
        }
        public void RemoveAt(int index)
        {
            int n = this._components.Length + 1;
            Element[] newarr = new Element[n];
            for (int i = 0; i < n - 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._components[i];
                else if (i == index - 1)
                    continue;
                else
                    newarr[i] = this._components[i + 1];
            }
            this._components = newarr;
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return _components.GetEnumerator();
        }
        public VisualScripting.BoundingBox GetUnionBounds()
        {
            VisualScripting.BoundingBox[] bbArray;
            bbArray = new VisualScripting.BoundingBox[this._components.Length];
            for (int i = 0; i < this._components.Length; i++) bbArray[i] = this._components[i].Bounds;
            return VisualScripting.BoundingBox.MassUnion(bbArray);
        }
    }

    
}
