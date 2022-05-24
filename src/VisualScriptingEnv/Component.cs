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
        private GUICLR.Rectangle nodeRectangle;
        private GUICLR.Text title;
        private GUICLR.Text valueText;

        private string titleStr;
        private float currentValue;
        private float maxValue;
        private float minValue;
        public Slider(string _title, float _currentValue, float _maxValue, float _minValue, float _zOrder = 0.00001f) : base(new CanvasPoint(0, 0), new CanvasSize(100, 100), _zOrder)
        {
            titleStr = _title;
            currentValue = _currentValue;
            maxValue = _maxValue;
            minValue = _minValue;

            setupBaseRendering();
        }

        private void setupBaseRendering()
        {
            baseRectangle = new GUICLR.Rectangle(Position.ToVector(), Size.ToVector(), 0.0f, 2.0f, 2.0f, 0.0f, Pivot.TOP_LEFT, Color.FromArgb(60, 60, 60));
            baseRectangle.setZDepth(ZOrder);

            float width = (float)Size.Width * (currentValue - minValue) / (maxValue - minValue);
            slideRectangle = new GUICLR.Rectangle(Position.ToVector(), new CanvasSize(width, Size.Height).ToVector(), 0.0f, 0.0f, 0.0f, 0.0f, Pivot.TOP_LEFT, Color.FromArgb(52, 186, 235));
            slideRectangle.setZDepth(ZOrder + 0.00001f);

            nodeRectangle = new GUICLR.Rectangle(new CanvasPoint(Position.X - Size.Height / 2.0f, Position.Y).ToVector(), new CanvasSize(Size.Height / 2.0f, Size.Height).ToVector(), (float)Size.Height / 2.0f, 0.0f, 0.0f, (float)Size.Height / 2.0f, Pivot.TOP_LEFT, Color.FromArgb(52, 186, 235));
            nodeRectangle.setZDepth(ZOrder + 0.00001f);

            title = new GUICLR.Text(titleStr + " :", Position.ToVector(), Size.ToVector(), (float)Size.Height, TextAlignment.LEFT, Pivot.TOP_LEFT);
            title.setZDepth(ZOrder + 0.00002f);

            valueText = new GUICLR.Text(CurrentValue.ToString(), Position.ToVector(), Size.ToVector(), (float)Size.Height, TextAlignment.RIGHT, Pivot.TOP_LEFT);
            valueText.setZDepth(ZOrder + 0.00002f);

            ZDepth = 0.00004f;
        }

        public string Title
        {
            get
            {
                return titleStr;
            }   
            
            set
            {
                titleStr = value;
                title.updateText(titleStr);
            }
        }
        public float CurrentValue
        {
            get
            {
                return currentValue;
            }

            set
            {
                currentValue = value;
                float width = (float)Size.Width * (currentValue - minValue) / (maxValue - minValue);
                slideRectangle.setSize(new CanvasSize(width, base.Size.Height).ToVector());
            }
        }
        public float MaxValue
        {
            get
            {
                return maxValue;
            }

            set
            {
                maxValue = value;
                float width = (float)Size.Width * (currentValue - minValue) / (maxValue - minValue);
                slideRectangle.setSize(new CanvasSize(width, base.Size.Height).ToVector());
            }
        }
        public float MinValue
        {
            get
            {
                return minValue;
            }

            set
            {
                minValue = value;
                float width = (float)Size.Width * (currentValue - minValue) / (maxValue - minValue);
                slideRectangle.setSize(new CanvasSize(width, base.Size.Height).ToVector());
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

                baseRectangle.setSize(base.Size.ToVector());

                float width = (float)Size.Width * (CurrentValue - MinValue) / (MaxValue - MinValue);
                slideRectangle.setSize(new CanvasSize(width, base.Size.Height).ToVector());

                nodeRectangle.setSize(new CanvasSize(Size.Height / 2.0f, Size.Height).ToVector());
                nodeRectangle.setRadius((float)Size.Height / 2.0f, 0.0f, 0.0f, (float)Size.Height / 2.0f);
                nodeRectangle.setPosition(new CanvasPoint(Position.X - Size.Height / 2.0f, Position.Y).ToVector());

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

                nodeRectangle.setPosition(new CanvasPoint(Position.X - Size.Height / 2.0f, Position.Y).ToVector());

                title.setPosition(new CanvasPoint(base.Position.X + 2.0f, base.Position.Y).ToVector());

                valueText.setPosition(base.Position.ToVector());
            }
        }

        public override float ZOrder 
        { get
            {
                return base.ZOrder;
            }

            set
            {
                base.ZOrder = value;
                baseRectangle.setZDepth(ZOrder);
                slideRectangle.setZDepth(ZOrder + 0.00001f);
                nodeRectangle.setZDepth(ZOrder + 0.00001f);
                title.setZDepth(ZOrder + 0.00002f);
                valueText.setZDepth(ZOrder + 0.00002f);
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
        private GUICLR.Icon icon;
        private Color color;

        private float nextHeight;

        public Component(string _title, float _zOrder = 0.00001f) : base(new CanvasPoint(0, 0), new CanvasSize(120, 100), _zOrder)
        {
            color = Color.FromArgb(255, 103, 0);
            title = _title;

            //this._instanceGuid = Guid.NewGuid();

            SetupBaseRendering();
        }

        public Component(string _title, Color _color, float _zOrder = 0.00001f) : base(new CanvasPoint(0,0), new CanvasSize(120,100), _zOrder)
        {
            color = _color;
            title = _title;

            //this._instanceGuid = Guid.NewGuid();

            SetupBaseRendering();
        }

        private void SetupBaseRendering()
        {
            backPlane = new GUICLR.Rectangle(Position.ToVector(), Size.ToVector(), 2.0f, 2.0f, 2.0f, 2.0f, Pivot.TOP_LEFT, Color.Gray);
            backPlane.setZDepth(ZOrder);

            accentColor = new GUICLR.Rectangle(Position.ToVector(), new CanvasSize(Size.Width, 5.0f).ToVector(), 2.0f, 2.0f, 0.0f, 0.0f, Pivot.TOP_LEFT, Color);
            accentColor.setZDepth(ZOrder + 0.00001f);

            titleText = new GUICLR.Text(title, new CanvasPoint(Position.X + 17.0f, Position.Y + 7.0f).ToVector(), new CanvasSize(Size.Width - 20.0f, 10.0f).ToVector(), 10.0f, TextAlignment.CENTER, Pivot.TOP_LEFT);
            titleText.setZDepth(ZOrder + 0.00001f);

            icon = new GUICLR.Icon("add.png", new CanvasPoint(Position.X + 2.0f, Position.Y + 7.0f).ToVector(), new CanvasSize(12.0f, 12.0f).ToVector(), Pivot.TOP_LEFT);
            icon.setZDepth(ZOrder + 0.00001f);

            ZDepth = 0.00003f;

            nextHeight = 30.0f;

        }

        public void AddSlider(string _title, float currentValue, float maxValue, float minValue)
        {
            Slider slider = new Slider(_title, currentValue, maxValue, minValue, ZOrder + 0.00001f);
            slider.Position = new CanvasPoint(0, nextHeight);
            slider.Size = new CanvasSize(Size.Width - 5.0f, 12.0f);
            slider.ZOrder = ZOrder + 0.00001f;

            this.Children.Add(slider);

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

                titleText.setSize(new CanvasSize(Size.Width - 20.0f, 10.0f).ToVector());
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

                titleText.setPosition(new CanvasPoint(Position.X + 17.0f, Position.Y + 7.0f).ToVector());

                icon.setPosition(new CanvasPoint(Position.X + 2.0f, Position.Y + 7.0f).ToVector());
            }
        }

        public override float ZOrder
        {
            get
            {
                return base.ZOrder;
            }
            set
            {
                base.ZOrder = value;
                backPlane.setZDepth(ZOrder);
                accentColor.setZDepth(ZOrder + 0.00001f);
                titleText.setZDepth(ZOrder + 0.00001f);
                icon.setZDepth(ZOrder + 0.00001f);
            }
        }

        public abstract override Guid ElementGuid { get; }
    }

    public abstract class Element : IDisposable
    {
        public event EventHandler OnMouseDown;

        protected Guid _instanceGuid;
        protected VisualScripting.BoundingBox boundingBox;
        //protected float zOrder;
        private float zDepth = 0.0f;

        protected List<Element> childElements = new List<Element>();

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

        public virtual float ZOrder
        {
            get
            {
                return ZOrderManager.ZPositions[this.InstanceGuid];
            }
            set
            {
                ZOrderManager.ZPositions[this.InstanceGuid] = value;

                foreach (Element elem in childElements)
                {
                    elem.ZOrder = ZOrderManager.ZPositions[this.InstanceGuid] + 0.00001f;
                }
            }
        }

        public float ZDepth
        {
            get
            {
                float temp = zDepth;

                foreach (Element elem in childElements)
                {
                    temp += elem.ZDepth;
                }

                return temp;
            }

            set
            {
                zDepth = value;
            }
        }

        public virtual void Translate(CanvasPoint translate)
        {
            Position += translate;

            foreach (Element elem in this.Children)
            {
                elem.Translate(translate);
            }
        }

        public Element(CanvasPoint position, CanvasSize size, float _zOrder = 0.00001f)
        {
            //_component = new GUICLR.Component("Element"); //!!!!!!!!!!!!!!!!!!!!!!!
            //_boundingBox = _component.getBoundingBox(); //!!!!!!!!!!!!!!!!!!!!!!!
            this._instanceGuid = Guid.NewGuid();
            boundingBox = new BoundingBox(position, size);

            Children = new ElementCollection(this);

            ZOrderManager.ZPositions.Add(this.InstanceGuid, _zOrder);
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
        public ElementCollection Children { get; }
        public string Name { get; set; }
        public string ElementType { get; set; }
        public abstract Guid ElementGuid { get; }

        public static bool operator == (Element A, Element B)
        {
            if (A is null) { if (B is null) return true; }
            else if (B is null) { if (A is null) return true; }
            return A.Equals(B);
        }
        public static bool operator != (Element A, Element B)
        {
            if (A is null) { if (B is null) return false; }
            else if (B is null) { if (A is null) return false; }
            return !A.Equals(B);
        }
        public override bool Equals(object obj)
        {
            if (!(obj is Element)) return false;
            Element c = obj as Element;
            return (this.InstanceGuid == c.InstanceGuid);
        }
        public bool Equals(Element other)
        {
            if (!(other is null)) return (this.InstanceGuid == other.InstanceGuid);
            else if (this is null) return true;
            else return false;
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
    public class ElementCollection : IList<Element>, ICollection<Element>, IEnumerable<Element>
    {
        private Element[] _elements = new Element[0];
        private Element _parent;
        private bool _readOnly;

        public ElementCollection() : base()
        {
            this._parent = null;
            this._readOnly = false;
        }
        public ElementCollection(Element owner) : base()
        {
            this._parent = owner;
            this._readOnly = false;
        }

        public Element this[int index]
        {
            get
            {
                return this._elements[index];
            }
            set
            {
                if (index < 0) throw new IndexOutOfRangeException("Index cannot be <0.");
                if (this._elements.Length > index)
                {
                    this._elements[index] = value;
                }
                else throw new IndexOutOfRangeException($"Index {index} is out of range [0 - {(this._elements.Length - 1)}].");
            }
        }

        public Element Owner => this._parent;
        public int Count => _elements.Length;

        public bool IsReadOnly => _readOnly;
        public void Add(Element item)
        {
            Element[] temp = this._elements;
            this._elements = new Element[temp.Length + 1];
            if (temp.Length > 0)
            {
                for (int i = 0; i < (temp.Length + 1); i++)
                {
                    if (i < temp.Length) this._elements[i] = temp[i];
                    else this._elements[i] = item;
                }
            }
            else
            {
                this._elements[0] = item;
            }
        }
        public void Clear()
        {
            _elements = new Element[] { };
        }
        public bool Contains(Element item)
        {
            for (int i = 0; i < this._elements.Length; i++)
            {
                if (item == this._elements[i])
                {
                    return true;
                }
            }
            return false;
        }
        public void CopyTo(Element[] array, int arrayIndex)
        {
            this._elements.CopyTo(array, arrayIndex);
        }
        public IEnumerator<Element> GetEnumerator()
        {
            foreach (Element element in _elements)
            {
                yield return element;
            }
        }
        public int IndexOf(Element item)
        {
            for (int i = 0; i < this._elements.Length; i++)
            {
                if (item == this._elements[i])
                {
                    return i;
                }
            }
            return -1;
        }
        public void Insert(int index, Element item)
        {
            int n = this._elements.Length + 1;
            Element[] newarr = new Element[n];
            for (int i = 0; i < n + 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._elements[i];
                else if (i == index - 1)
                    newarr[i] = item;
                else
                    newarr[i] = this._elements[i - 1];
            }
            this._elements = newarr;
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
            int n = this._elements.Length + 1;
            Element[] newarr = new Element[n];
            for (int i = 0; i < n - 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._elements[i];
                else if (i == index - 1)
                    continue;
                else
                    newarr[i] = this._elements[i + 1];
            }
            this._elements = newarr;
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return _elements.GetEnumerator();
        }
        public VisualScripting.BoundingBox GetUnionBounds()
        {
            VisualScripting.BoundingBox[] bbArray;
            bbArray = new VisualScripting.BoundingBox[this._elements.Length];
            for (int i = 0; i < this._elements.Length; i++) bbArray[i] = this._elements[i].Bounds;
            return VisualScripting.BoundingBox.MassUnion(bbArray);
        }
    }

    
}
