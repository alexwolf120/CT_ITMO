package markup;
import java.util.List;
public class Strong extends absclass {
    public Strong(List<MarkUp> marup) {
        super(marup);
    }
    public void toMarkdown(StringBuilder str) {
        super.toMarkdown(str,"__");
    }
    public void toHtml(StringBuilder str) {
        super.toHtml(str,"<strong>","</strong>");
    }

}