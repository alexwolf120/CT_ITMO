package markup;

import java.util.List;

public abstract class absclass extends Paragraph {
    public absclass(List<MarkUp> marup) {
        super(marup);
    }
    public void toMarkdown(StringBuilder str, String variable) {
        str.append(variable);
        for (MarkUp md : marup) {
            md.toMarkdown(str);
        }
        str.append(variable);
    }
    public void toHtml(StringBuilder str, String variable1, String variable2) {
        str.append(variable1);
        for (MarkUp hl : marup) {
            hl.toHtml(str);
        }
        str.append(variable2);
    }
}