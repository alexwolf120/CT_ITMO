package markup;

public class Text extends MarkUp {
    private String text;

    public Text(String text) {
        this.text = text;
    }
    public void toMarkdown(StringBuilder str) {
        str.append(text);
    }
    public void toHtml(StringBuilder str) {
        str.append(text);
    }
}