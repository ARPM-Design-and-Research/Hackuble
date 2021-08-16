using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GUICLR;

namespace VisualScripting
{
    public class Component : IDisposable
    {

        GUICLR.Component _component;
        GUICLR.BoundingBox _boundingBox;
        public Component()
        {
            _component = new GUICLR.Component("Component");
            _boundingBox = _component.getBoundingBox();
        }

        //BoundingBox

        public void addSlider(string title, SliderState sliderState, float currentValue, float startValue, float endValue)
        {
            _component.addSlider(title, sliderState, currentValue, startValue, endValue);
        }

        public void setColor(int r, int g, int b)
        {
            _component.setColor(r, g, b);
        }

        public string getName()
        {
            return _component.getTitle();
        }

        public void setName(string name)
        {
            _component.setTitle(name);
        }

        public ComponentCollection Children { get; }

        public void Dispose()
        {

        }

        ~Component()
        {
            this.Dispose();
        }
    }

    public class ComponentCollection : IList<Component>, ICollection<Component>, IEnumerable<Component>
    {
        private Component[] _components;
        private Component _parent;



        //BoundingBox

        public Component this[int index] { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public int Count => _components.Length;

        public bool IsReadOnly => false;

        public void Add(Component item)
        {
            throw new NotImplementedException();
        }

        public void Clear()
        {
            _components = new Component[] { };
        }

        public bool Contains(Component item)
        {
            throw new NotImplementedException();
        }

        public void CopyTo(Component[] array, int arrayIndex)
        {
            throw new NotImplementedException();
        }

        public IEnumerator<Component> GetEnumerator()
        {
            return (IEnumerator<Component>)_components.GetEnumerator();
        }

        public int IndexOf(Component item)
        {
            throw new NotImplementedException();
        }

        public void Insert(int index, Component item)
        {
            throw new NotImplementedException();
        }

        public bool Remove(Component item)
        {
            throw new NotImplementedException();
        }

        public void RemoveAt(int index)
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _components.GetEnumerator();
        }
    }
}
