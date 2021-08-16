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
        }

        public override Guid ElementGuid => new Guid("733d497c-d74e-4b86-adde-86b61c45b87a");
    }

    public abstract class Component : Element
    {
        private Color _color;

        public Component(string title)
        {
            _component = new GUICLR.Component(title);
            _color = Color.FromArgb(255, 103, 0);
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
            this._instanceGuid = Guid.NewGuid();
        }
        public Component(string title, Color color)
        {
            _component = new GUICLR.Component(title);
            _color = color;
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
            this._instanceGuid = Guid.NewGuid();
        }

        public Color Color
        {
            get
            {
                return _color;
            }
            set
            {
                _color = value;
                int r, g, b;
                r = ((int)_color.R);
                g = ((int)_color.G);
                b = ((int)_color.B);
                _component.setColor(r, g, b);
            }
        }

        public abstract override Guid ElementGuid { get; }

        public void AddSlider(SliderState sliderState, string title, float currentValue, float startValue, float endValue) //!!!!!!!!!!!!!!!!!!!!!!!
        {
            _component.addSlider(title, sliderState, currentValue, startValue, endValue);
        }
    }

    public abstract class Element : IDisposable
    {
        protected GUICLR.Component _component; //!!!!!!!!!!!!!!!!!!!!!!!
        protected GUICLR.BoundingBox _boundingBox; //!!!!!!!!!!!!!!!!!!!!!!!
        protected Guid _instanceGuid;

        public Element()
        {
            //_component = new GUICLR.Component("Element"); //!!!!!!!!!!!!!!!!!!!!!!!
            //_boundingBox = _component.getBoundingBox(); //!!!!!!!!!!!!!!!!!!!!!!!
            this._instanceGuid = Guid.NewGuid();
        }        

        public VisualScriptingEnv.BoundingBox Bounds //!!!!!!!!!!!!!!!!!!!!!!!
        {
            get
            {
                //return this._boundingBox;
                return VisualScriptingEnv.BoundingBox.FromLTRB((double)this._boundingBox.getLeftBound(), (double)this._boundingBox.getTopBound(), (double)this._boundingBox.getRightBound(), (double)this._boundingBox.getBottomBound());
            }
        }
        public Guid InstanceGuid { get => _instanceGuid; }
        public ComponentCollection Children { get; }
        public string Name //!!!!!!!!!!!!!!!!!!!!!!!
        {
            get
            {
                return _component.getTitle();
            }
            set
            {
                _component.setTitle(value);
            }
        }
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
            _component.Dispose();
        }
        ~Element()
        {
            this.Dispose();
        }
    }

    public class ComponentCollection : IList<Element>, ICollection<Element>, IEnumerable<Element>
    {
        private Element[] _components;
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
            return (IEnumerator<Element>)this._components.GetEnumerator();
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
        public VisualScriptingEnv.BoundingBox GetUnionBounds()
        {
            VisualScriptingEnv.BoundingBox[] bbArray;
            bbArray = new VisualScriptingEnv.BoundingBox[this._components.Length];
            for (int i = 0; i < this._components.Length; i++) bbArray[i] = this._components[i].Bounds;
            return VisualScriptingEnv.BoundingBox.MassUnion(bbArray);
        }
    }

    
}
